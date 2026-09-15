#include <vap/streaming/session/streaming_session.hpp>

#include <QThread>

#include <vap/streaming/worker/streaming_worker.hpp>
#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>
#include <vap/streaming/frame/ffmpeg_frame_converter.hpp>
#include <vap/camera/camera_config.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>

namespace vap
{

StreamingSession::StreamingSession(QObject* parent)
: QObject(parent)
{
    m_streamingService = std::make_unique<FFmpegStreamingService>(std::make_unique<FFmpegFrameConverter>());
    m_streamingWorker = std::make_unique<StreamingWorker>(m_streamingService.get(), m_frameExchange);
    m_streamingThread = std::make_unique<QThread>();

    connect(m_streamingWorker.get(),
            &StreamingWorker::frameUpdated,
            this,
            &StreamingSession::frameUpdated);

    connect(
        m_streamingWorker.get(),
        &StreamingWorker::stateChanged,
        this,
        &StreamingSession::onStateChanged);

    connect(
        m_streamingWorker.get(),
        &StreamingWorker::errorOccurred,
        this,
        &StreamingSession::errorOccurred);

    connect(
        m_streamingService.get(),
        &IStreamingService::statisticsUpdated,
        this,
        &StreamingSession::onStatisticsUpdated);

    connect(
        m_streamingWorker.get(),
        &StreamingWorker::recordingStateChanged,
        this,
        &StreamingSession::onRecordingStateChanged);

    connect(
        m_streamingService.get(),
        &IStreamingService::recordingDurationChanged,
        this,
        &StreamingSession::recordingDurationChanged);

    m_streamingService->moveToThread(m_streamingThread.get());
    m_streamingWorker->moveToThread(m_streamingThread.get());

    m_streamingThread->start();
}

StreamingSession::~StreamingSession()
{
    m_stopSource.request_stop();

    m_streamingThread->quit();
    m_streamingThread->wait();
}

void StreamingSession::start(const Camera& camera)
{
    Q_ASSERT(QThread::currentThread() == thread());

    // Cancel the previous active or queued run.
    m_stopSource.request_stop();

    // Give the new run an independent cancellation state.
    m_stopSource = std::stop_source{};

    m_camera = camera;

    auto* worker = m_streamingWorker.get();
    const auto token = m_stopSource.get_token();

    QMetaObject::invokeMethod(
        worker,
        [worker, uri = camera.config.url, token]
        {
            worker->start(uri, token);
        },
        Qt::QueuedConnection);
}

void StreamingSession::stop()
{
    Q_ASSERT(QThread::currentThread() == thread());

    m_stopSource.request_stop();
}

void StreamingSession::startRecording(const RecordingConfiguration &configuration)
{
    m_streamingService->enqueueStartRecording(configuration);
}

void StreamingSession::stopRecording()
{
    m_streamingService->enqueueStopRecording();
}

RecordingState StreamingSession::recordingState() const noexcept
{
    return m_recordingState;
}

ConnectionState StreamingSession::state() const
{
    return m_state;
}

const StreamStatistics& StreamingSession::statistics() const
{
    return m_statistics;
}

QString StreamingSession::cameraName() const
{
    return m_camera.name;
}

FrameSnapshot StreamingSession::currentFrame() const
{
    return m_frameExchange.snapshot();
}

void StreamingSession::onStateChanged(const ConnectionState &state)
{
    if(m_state == state)
        return;
    m_state = state;
    emit stateChanged(m_state);
}

void StreamingSession::onStatisticsUpdated(
    const StreamStatistics& statistics)
{
    m_statistics = statistics;
    emit statisticsUpdated(m_statistics);
}

void StreamingSession::onRecordingStateChanged(RecordingState state)
{
    if (m_recordingState == state)
        return;

    m_recordingState = state;

    emit recordingStateChanged(state);
}

}
