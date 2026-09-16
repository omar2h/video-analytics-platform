#include <vap/analytics/analytics_session.hpp>
#include <vap/analytics/detection/onnx/onnx_yolox_detector.hpp>
#include <vap/camera/camera.hpp>
#include <vap/streaming/session/streaming_session.hpp>

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include <chrono>
#include <cstdint>
#include <exception>
#include <memory>
#include <utility>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    if (argc != 3)
    {
        qCritical() << "Usage: analytics_live_demo <model.onnx> <rtsp-url>";
        return 1;
    }

    try
    {
        vap::OnnxYoloXConfiguration configuration;
        configuration.modelPath = argv[1];

        vap::AnalyticsSession analytics(
            [configuration]
            {
                return std::make_unique<vap::OnnxYoloXDetector>(
                    configuration);
            });

        // Declared after analytics: streaming is destroyed first.
        vap::StreamingSession streaming;

        vap::Camera camera;
        camera.id = QStringLiteral("demo-camera");
        camera.name = QStringLiteral("Live analytics demo");
        camera.config.url = QString::fromLocal8Bit(argv[2]);

        // This demo starts exactly one run, with no restart controls.
        constexpr std::uint64_t runId = 1;
        const auto generation = analytics.enable(runId);

        QObject::connect(
            &analytics,
            &vap::AnalyticsSession::errorOccurred,
            &app,
            [&](const QString& message)
            {
                qCritical().noquote() << "Analytics failed:" << message;
                app.exit(1);
            });

        QObject::connect(
            &analytics,
            &vap::AnalyticsSession::resultAvailable,
            &app,
            [&]
            {
                auto result = analytics.takeLatestResult();

                if (!result)
                    return;

                const double milliseconds =
                    std::chrono::duration<double, std::milli>(
                        result->inferenceDuration).count();

                qInfo() << "Frame:" << result->identity.frameRevision
                        << "Detections:" << result->detections.size()
                        << "Inference ms:" << milliseconds;

                for (const auto& detection : result->detections)
                {
                    qInfo() << "  class:" << detection.classId
                            << "score:" << detection.score
                            << "box:"
                            << detection.box.x
                            << detection.box.y
                            << detection.box.width
                            << detection.box.height;
                }
            });

        std::uint64_t lastRevision = 0;
        QTimer sampleTimer;

        QObject::connect(
            &sampleTimer,
            &QTimer::timeout,
            &app,
            [&]
            {
                const auto snapshot = streaming.currentFrame();

                if (!snapshot.valid ||
                    snapshot.revision == lastRevision)
                {
                    return;
                }

                lastRevision = snapshot.revision;

                vap::AnalyticsFrame frame;
                frame.identity = {
                    runId, generation, snapshot.revision
                };
                frame.decodeTime = snapshot.decodeTime;
                frame.image = snapshot.image;

                const bool accepted =
                    analytics.submitFrame(std::move(frame));

                if (!accepted)
                    qWarning() << "Analytics rejected a sampled frame.";
            });

        streaming.start(camera);
        sampleTimer.start(100);

        QTimer::singleShot(30'000, &app, &QCoreApplication::quit);

        const int exitCode = app.exec();

        sampleTimer.stop();
        analytics.disable();
        streaming.stop();

        return exitCode;
    }
    catch (const std::exception& error)
    {
        qCritical() << error.what();
        return 1;
    }
}
