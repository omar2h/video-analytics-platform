#include <vap/streaming/worker/streaming_worker.hpp>

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

    const auto result = m_streamingService->stream(uri);

    switch(result)
    {
    case StreamingExitReason::Cancelled:
        emit stateChanged(ConnectionState::Disconnected);
        break;

    case StreamingExitReason::StreamEnded:
        emit stateChanged(ConnectionState::Disconnected);
        break;

    case StreamingExitReason::UnknownError:
        emit stateChanged(ConnectionState::Error);
        break;
    }
}

void StreamingWorker::requestCancellation()
{
    m_streamingService->requestCancellation();
}

}
