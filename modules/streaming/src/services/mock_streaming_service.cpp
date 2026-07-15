#include <vap/streaming/services/mock_streaming_service.hpp>

#include <QTimer>

namespace vap
{

MockStreamingService::MockStreamingService(QObject* parent) : IStreamingService(parent)
{
}

void MockStreamingService::connectToStream(const QString& uri)
{

}

void MockStreamingService::requestCancellation()
{
    emit stateChanged(ConnectionState::Disconnected);
}

}
