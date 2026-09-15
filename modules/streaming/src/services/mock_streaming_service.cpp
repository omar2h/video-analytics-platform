#include <vap/streaming/services/mock_streaming_service.hpp>

#include <QTimer>

namespace vap
{

MockStreamingService::MockStreamingService(QObject* parent) : IStreamingService(parent)
{
}

StreamingExitReason MockStreamingService::stream(const QString&, std::stop_token stopToken)
{
    return stopToken.stop_requested()
               ? StreamingExitReason::Cancelled
               : StreamingExitReason::StreamEnded;
}


}
