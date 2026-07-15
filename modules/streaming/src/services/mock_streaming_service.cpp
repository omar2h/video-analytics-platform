#include <vap/streaming/services/mock_streaming_service.hpp>

#include <QTimer>

namespace vap
{

MockStreamingService::MockStreamingService(QObject* parent) : IStreamingService(parent)
{
}

StreamingExitReason MockStreamingService::stream(const QString& uri)
{
    return StreamingExitReason::StreamEnded;
}

void MockStreamingService::requestCancellation()
{
}

}
