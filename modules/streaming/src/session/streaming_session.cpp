#include <vap/streaming/session/streaming_session.hpp>

#include <QThread>
#include <QDebug>

#include <vap/streaming/worker/streaming_worker.hpp>
#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>
#include <vap/streaming/frame/ffmpeg_frame_converter.hpp>
#include <vap/camera/camera_config.hpp>

namespace vap
{

StreamingSession::StreamingSession(QObject* parent)
{
    m_streamingService = std::make_unique<FFmpegStreamingService>(std::make_unique<FFmpegFrameConverter>());
    m_streamingWorker = std::make_unique<StreamingWorker>(m_streamingService.get());
    m_streamingThread = std::make_unique<QThread>();

    // connect(
    //     m_streamingWorker.get(),
    //     &StreamingWorker::frameReady,
    //     this,
    //     &StreamingSession::frameReady);

    connect(m_streamingWorker.get(),
            &StreamingWorker::frameReady,
            this,
            [this](const QImage& image)
            {
                qDebug() << "StreamingSession::frameReady";

                emit frameReady(image);
            });

    connect(
        m_streamingWorker.get(),
        &StreamingWorker::stateChanged,
        this,
        &StreamingSession::stateChanged);

    connect(
        m_streamingWorker.get(),
        &StreamingWorker::errorOccurred,
        this,
        &StreamingSession::errorOccurred);

    m_streamingService->moveToThread(m_streamingThread.get());
    m_streamingWorker->moveToThread(m_streamingThread.get());

    m_streamingThread->start();
}

StreamingSession::~StreamingSession()
{
    m_streamingWorker->requestCancellation();

    m_streamingThread->quit();
    m_streamingThread->wait();
}

void StreamingSession::start(const CameraConfig& config)
{
    QMetaObject::invokeMethod(
        m_streamingWorker.get(),
        "start",
        Qt::QueuedConnection,
        Q_ARG(QString, config.url));
}

void StreamingSession::stop()
{
    m_streamingWorker->requestCancellation();
}

}
