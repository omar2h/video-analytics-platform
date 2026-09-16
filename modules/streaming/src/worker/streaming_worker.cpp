#include <vap/streaming/worker/streaming_worker.hpp>

#include <vap/streaming/services/i_streaming_service.hpp>
#include <vap/streaming/frame/frame_exchange.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

namespace vap
{

StreamingWorker::StreamingWorker(
    IStreamingService* streamingService,
    FrameExchange& frameExchange,
    QObject* parent)
    : QObject(parent)
    , m_streamingService(streamingService),
    m_frameExchange(frameExchange)
{
    Q_ASSERT(m_streamingService != nullptr);

    connect(m_streamingService,
        &IStreamingService::connected, this,
        [this]
        {
            m_reconnectPolicy.reset();
            emit stateChanged(ConnectionState::Connected);
        }
    );

    connect(m_streamingService,
            &IStreamingService::frameReady,
            this,
            [this](const QImage& frame){
                m_frameExchange.publish(
                    frame,
                    std::chrono::steady_clock::now(),
                    m_streamRunId);
                emit frameUpdated();
            });

    connect(
        m_streamingService,
        &IStreamingService::recordingStateChanged,
        this,
        &StreamingWorker::recordingStateChanged);
}

void StreamingWorker::start(
    const QString& uri,
    std::stop_token stopToken,
    std::uint64_t streamRunId){
    if (stopToken.stop_requested())
    {
        emit stateChanged(ConnectionState::Disconnected);
        return;
    }

    m_streamRunId = streamRunId;

    m_reconnectPolicy.reset();
    emit stateChanged(ConnectionState::Connecting);

    while (!stopToken.stop_requested())
    {
        const auto reason =
            m_streamingService->stream(uri, stopToken);

        if (stopToken.stop_requested())
            break;

        if (!handleExitReason(reason))
            break;

        if (!m_reconnectPolicy.shouldRetry())
        {
            emit stateChanged(ConnectionState::Error);
            break;
        }

        if (!waitForRetryDelay(stopToken))
            break;

        m_reconnectPolicy.recordRetry();
    }

    if (stopToken.stop_requested())
    {
        emit stateChanged(ConnectionState::Disconnected);
    }
}

bool StreamingWorker::handleExitReason(StreamingExitReason reason)
{
    switch (reason)
    {
    case StreamingExitReason::Cancelled:
        return false;

    case StreamingExitReason::InitializationFailure:
        emit stateChanged(ConnectionState::Reconnecting);
        return true;

    case StreamingExitReason::StreamEnded:
        emit stateChanged(ConnectionState::Disconnected);
        return false;

    case StreamingExitReason::NetworkFailure:
        emit stateChanged(ConnectionState::Reconnecting);
        return true;

    case StreamingExitReason::UnknownError:
        emit stateChanged(ConnectionState::Error);
        return false;
    }
    return false;
}

bool StreamingWorker::waitForRetryDelay(
    std::stop_token stopToken)
{
    std::unique_lock<std::mutex> lock(m_waitMutex);

    m_waitCondition.wait_for(
        lock,
        stopToken,
        m_reconnectPolicy.retryDelay(),
        [] { return false; });

    return !stopToken.stop_requested();
}

}
