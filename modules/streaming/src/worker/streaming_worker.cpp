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
            &IStreamingService::frameReady,
            this,
            &StreamingWorker::frameReady);
}

void StreamingWorker::start(const QString& uri)
{
    m_streamingService->stream(uri);
}

void StreamingWorker::requestCancellation()
{
    m_streamingService->requestCancellation();
}

}
