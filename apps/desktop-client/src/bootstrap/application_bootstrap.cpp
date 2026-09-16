#include "application_bootstrap.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>

#include <vap/streaming/services/mock_streaming_service.hpp>
#include <vap/camera/repositories/sqlite_camera_repository.hpp>
#include <vap/camera/validation/camera_validator.hpp>
#include <vap/camera/services/camera_application_service.hpp>
#include <vap/database/database.hpp>
#include <vap/streaming/session/streaming_session.hpp>
#include <vap/streaming/manager/streaming_manager.hpp>
#include <vap/common/connection_state.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

#include <src/providers/video_frame_provider.hpp>

#include <src/viewmodels/camera_stream_view_model.hpp>
#include <src/viewmodels/camera_management_view_model.hpp>
#include <src/viewmodels/live_monitoring_view_model.hpp>

#ifdef VAP_HAS_ANALYTICS
#include <vap/analytics/analytics_session.hpp>
#include <vap/analytics/detection/onnx/onnx_yolox_detector.hpp>

#include <QDebug>
#include <QPointer>
#include <QTimer>

#include <chrono>
#include <cstdint>
#include <optional>
#include <utility>
#endif

namespace vap
{
ApplicationBootstrap::ApplicationBootstrap(QQmlApplicationEngine& engine)
    : m_engine(engine) {}

ApplicationBootstrap::~ApplicationBootstrap() = default;

void ApplicationBootstrap::initialize()
{
    qRegisterMetaType<vap::StreamStatistics>();
    qRegisterMetaType<vap::RecordingConfiguration>();

    m_streamingManager = std::make_unique<StreamingManager>();
    m_database = std::make_unique<Database>("video_analytics.db");

    if (!m_database->open())
    {
        qFatal("Failed to open database.");
    }

    m_cameraRepository = std::make_unique<SQLiteCameraRepository>(*m_database);

    for (const auto& camera : m_cameraRepository->cameras())
    {
        m_streamingManager->createSession(camera.id);
    }

    m_cameraValidator = std::make_unique<CameraValidator>();
    m_cameraApplicationService = std::make_unique<CameraApplicationService>(m_cameraRepository.get(), m_cameraValidator.get(), m_streamingManager.get());
    m_cameraManagementViewModel = std::make_unique<CameraManagementViewModel>(m_cameraApplicationService.get(), m_streamingManager.get());


    m_videoFrameProvider = new VideoFrameProvider();

    m_engine.addImageProvider(
        "video",
        m_videoFrameProvider);

    m_liveMonitoringViewModel = std::make_unique<LiveMonitoringViewModel>(m_streamingManager.get(), m_videoFrameProvider);

#ifdef VAP_HAS_ANALYTICS
    const QString modelPath =
        qEnvironmentVariable("VAP_ANALYTICS_MODEL");

    const auto cameras = m_cameraRepository->cameras();

    if (!modelPath.isEmpty() && !cameras.isEmpty())
    {
        const QString cameraId = cameras.front().id;

        QPointer<StreamingSession> stream =
            m_streamingManager->session(cameraId);

        QPointer<CameraStreamViewModel> viewModel =
            m_liveMonitoringViewModel->streamViewModel(cameraId);

        OnnxYoloXConfiguration configuration;
        configuration.modelPath = modelPath.toStdString();

        auto* analytics = new AnalyticsSession(
            [configuration]
            {
                return std::make_unique<OnnxYoloXDetector>(
                    configuration);
            },
            m_liveMonitoringViewModel.get());

        auto* timer = new QTimer(analytics);

        struct OverlayState
        {
            std::uint64_t runId{};
            std::uint64_t generation{};
            std::uint64_t lastRevision{};
            bool failed{false};

            std::optional<std::chrono::steady_clock::time_point>
                displayedFrameTime;
        };

        // Shared by the timer and result callbacks.
        const auto state = std::make_shared<OverlayState>();

        QObject::connect(
            analytics,
            &AnalyticsSession::errorOccurred,
            analytics,
            [state, viewModel, timer](const QString& message)
            {
                state->failed = true;
                timer->stop();

                if (viewModel)
                    viewModel->clearDetections();

                qWarning().noquote()
                    << "Camera analytics failed:" << message;
            });

        QObject::connect(
            analytics,
            &AnalyticsSession::resultAvailable,
            analytics,
            [analytics, state, stream, viewModel]
            {
                auto result = analytics->takeLatestResult();

                if (!result || !stream || !viewModel)
                    return;

                const auto age =
                    std::chrono::steady_clock::now()
                    - result->decodeTime;

                if (stream->state() != ConnectionState::Connected ||
                    result->identity.streamRunId != stream->streamRunId() ||
                    result->identity.analyticsGeneration != state->generation ||
                    age > std::chrono::milliseconds(750))
                {
                    return;
                }

                QList<DetectionOverlayItem> items;

                for (const auto& detection : result->detections)
                {
                    // Person label for the current demonstration;
                    // other classes remain explicitly numbered.
                    const QString label = detection.classId == 0
                                              ? QStringLiteral("person")
                                              : QStringLiteral("class %1")
                                                    .arg(detection.classId);

                    items.push_back({
                        detection.box.x,
                        detection.box.y,
                        detection.box.width,
                        detection.box.height,
                        label,
                        detection.score
                    });
                }

                state->displayedFrameTime = result->decodeTime;

                viewModel->updateDetections(
                    std::move(items),
                    result->imageSize);
            });

        QObject::connect(
            timer,
            &QTimer::timeout,
            analytics,
            [analytics, timer, state, stream, viewModel]
            {
                if (!stream || !viewModel)
                {
                    timer->stop();
                    analytics->disable();
                    return;
                }

                if (state->failed)
                    return;

                const auto now = std::chrono::steady_clock::now();

                if (state->displayedFrameTime &&
                    now - *state->displayedFrameTime >
                        std::chrono::milliseconds(750))
                {
                    viewModel->clearDetections();
                    state->displayedFrameTime.reset();
                }

                const auto runId = stream->streamRunId();

                if (runId == 0 ||
                    stream->state() != ConnectionState::Connected)
                {
                    analytics->disable();
                    viewModel->clearDetections();

                    state->runId = 0;
                    state->lastRevision = 0;
                    state->displayedFrameTime.reset();
                    return;
                }

                if (state->runId != runId)
                {
                    viewModel->clearDetections();

                    state->generation = analytics->enable(runId);
                    state->runId = runId;
                    state->lastRevision = 0;
                    state->displayedFrameTime.reset();
                }

                const auto snapshot = stream->currentFrame();

                if (!snapshot.valid ||
                    snapshot.streamRunId != runId ||
                    snapshot.revision == state->lastRevision)
                {
                    return;
                }

                state->lastRevision = snapshot.revision;

                AnalyticsFrame frame;
                frame.identity = {
                    snapshot.streamRunId,
                    state->generation,
                    snapshot.revision
                };
                frame.decodeTime = snapshot.decodeTime;
                frame.image = snapshot.image;

                const bool accepted =
                    analytics->submitFrame(std::move(frame));

                // Rejection can happen when the worker reports failure.
                Q_UNUSED(accepted);
            });

        timer->start(100);

        qInfo() << "Analytics attached to camera:" << cameras.front().name;
    }
#endif

    m_engine.rootContext()->setContextProperty("cameraManagementViewModel", m_cameraManagementViewModel.get());
    m_engine.rootContext()->setContextProperty("liveMonitoringViewModel", m_liveMonitoringViewModel.get());

    qmlRegisterUncreatableMetaObject(
        vap::staticMetaObject,
        "VAP",
        1,
        0,
        "ConnectionState",
        "Enum only");
}

}
