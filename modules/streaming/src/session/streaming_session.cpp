#include <vap/streaming/session/streaming_session.hpp>

#include <QThread>

#include <limits>
#include <stdexcept>

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
    m_streamingThread = std::make_unique<QThread>();

    auto service = std::make_unique<FFmpegStreamingService>(std::make_unique<FFmpegFrameConverter>());

    auto worker = std::make_unique<StreamingWorker>(service.get(), m_frameExchange);

    m_streamingService = service.get();
    m_streamingWorker = worker.get();

    connect(m_streamingWorker,
            &StreamingWorker::frameUpdated,
            this,
            &StreamingSession::frameUpdated);

    connect(
        m_streamingWorker,
        &StreamingWorker::stateChanged,
        this,
        &StreamingSession::onStateChanged);

    connect(
        m_streamingWorker,
        &StreamingWorker::errorOccurred,
        this,
        &StreamingSession::errorOccurred);

    connect(
        m_streamingService,
        &IStreamingService::statisticsUpdated,
        this,
        &StreamingSession::onStatisticsUpdated);

    connect(
        m_streamingWorker,
        &StreamingWorker::recordingStateChanged,
        this,
        &StreamingSession::onRecordingStateChanged);

    connect(
        m_streamingService,
        &IStreamingService::recordingDurationChanged,
        this,
        &StreamingSession::recordingDurationChanged);

    connect(
        m_streamingThread.get(),
        &QThread::finished,
        m_streamingWorker,
        &QObject::deleteLater);

    // Transfer service ownership to the worker.
    service->setParent(worker.get());
    service.release();

    // Moving the worker also moves its child service.
    worker->moveToThread(m_streamingThread.get());

    // Thread-finish cleanup now controls the worker's deletion.
    worker.release();

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

    if (m_streamRunId ==
        std::numeric_limits<std::uint64_t>::max())
    {
        throw std::overflow_error("Stream run counter exhausted.");
    }

    m_stopSource.request_stop();
    m_stopSource = std::stop_source{};

    m_camera = camera;
    const auto runId = ++m_streamRunId;

    auto* worker = m_streamingWorker;
    const auto token = m_stopSource.get_token();

    QMetaObject::invokeMethod(
        worker,
        [worker, uri = camera.config.url, token, runId]
        {
            worker->start(uri, token, runId);
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
    const auto runId = streamRunId();
    auto snapshot = m_frameExchange.snapshot();

    if (runId == 0 || snapshot.streamRunId != runId)
        return {};

    return snapshot;
}

std::uint64_t StreamingSession::streamRunId() const
{
    Q_ASSERT(QThread::currentThread() == thread());

    // Zero means no active requested run.
    return m_stopSource.stop_requested() ? 0 : m_streamRunId;
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
