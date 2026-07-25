#include <vap/streaming/worker/streaming_worker.hpp>

#include <vap/streaming/services/i_streaming_service.hpp>
#include <vap/streaming/frame/frame_exchange.hpp>

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
                m_frameExchange.publish(frame);
                emit frameUpdated();
            });
}

void StreamingWorker::start(const QString& uri)
{
    m_cancelRequested.store(false);
    m_reconnectPolicy.reset();
    emit stateChanged(ConnectionState::Connecting);
    while (!m_cancelRequested.load())
    {
        const auto reason = m_streamingService->stream(uri);

        if (!handleExitReason(reason))
            break;

        if (!m_reconnectPolicy.shouldRetry())
        {
            emit stateChanged(ConnectionState::Error);
            break;
        }

        if (!waitForRetryDelay())
        {
            break;
        }

        m_reconnectPolicy.recordRetry();
    }
    if (m_cancelRequested.load())
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

bool StreamingWorker::waitForRetryDelay()
{
    QMutexLocker locker(&m_waitMutex);

    if (m_cancelRequested.load())
    {
        return false;
    }

    const auto timeout =
        static_cast<unsigned long>(
            m_reconnectPolicy.retryDelay().count());

    m_waitCondition.wait(&m_waitMutex, timeout);

    return !m_cancelRequested.load();
}

void StreamingWorker::requestCancellation()
{
    m_cancelRequested.store(true);

    m_waitCondition.wakeAll();

    m_streamingService->requestCancellation();
}

}
