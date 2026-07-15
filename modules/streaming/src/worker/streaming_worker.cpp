#include <vap/streaming/worker/streaming_worker.hpp>

#include <QThread>

#include <vap/streaming/services/i_streaming_service.hpp>

namespace vap
{

StreamingWorker::StreamingWorker(
    IStreamingService* streamingService,
    QObject* parent)
    : QObject(parent)
    , m_streamingService(streamingService)
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
            &StreamingWorker::frameReady);
}

void StreamingWorker::start(const QString& uri)
{
    emit stateChanged(ConnectionState::Connecting);
    while (true)
    {
        const auto reason = m_streamingService->stream(uri);

        if (!handleExitReason(reason))
            break;

        if (!m_reconnectPolicy.shouldRetry())
        {
            emit stateChanged(ConnectionState::Error);
            break;
        }

        QThread::msleep(
            m_reconnectPolicy.retryDelay().count());

        m_reconnectPolicy.recordRetry();
    }
}

bool StreamingWorker::handleExitReason(StreamingExitReason reason)
{
    switch (reason)
    {
    case StreamingExitReason::Cancelled:
        emit stateChanged(ConnectionState::Disconnected);
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

void StreamingWorker::requestCancellation()
{
    m_streamingService->requestCancellation();
}

}
