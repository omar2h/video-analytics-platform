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

#include <src/providers/video_frame_provider.hpp>

#include <src/viewmodels/camera_stream_view_model.hpp>
#include <src/viewmodels/camera_management_view_model.hpp>
#include <src/viewmodels/live_monitoring_view_model.hpp>


namespace vap
{
ApplicationBootstrap::ApplicationBootstrap(QQmlApplicationEngine& engine)
    : m_engine(engine) {}

ApplicationBootstrap::~ApplicationBootstrap() = default;

void ApplicationBootstrap::initialize()
{
    m_streamingManager = std::make_unique<StreamingManager>();
    m_database = std::make_unique<Database>("video_analytics.db");

    if (!m_database->open())
    {
        qFatal("Failed to open database.");
    }

    m_cameraRepository = std::make_unique<SQLiteCameraRepository>(*m_database);

    m_cameraValidator = std::make_unique<CameraValidator>();
    m_cameraApplicationService = std::make_unique<CameraApplicationService>(m_cameraRepository.get(), m_cameraValidator.get());
    m_cameraManagementViewModel = std::make_unique<CameraManagementViewModel>(m_cameraApplicationService.get(), m_streamingManager.get());


    m_videoFrameProvider = new VideoFrameProvider();

    m_engine.addImageProvider(
        "video",
        m_videoFrameProvider);

    m_liveMonitoringViewModel = std::make_unique<LiveMonitoringViewModel>(m_streamingManager.get(), m_videoFrameProvider);

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
