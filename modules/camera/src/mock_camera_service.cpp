#include <vap/camera/mock_camera_service.hpp>

#include <QTimer>

namespace vap
{

MockCameraService::MockCameraService(
    QObject* parent)
    : ICameraService(parent)
{
}

void MockCameraService::connect(const CameraConfig& config)
{
    Q_UNUSED(config);

    emit stateChanged(StreamState::Connecting);

    QTimer::singleShot(
        2000,
        this,
        [this]()
        {
            emit stateChanged(StreamState::Connected);
        });
}

void MockCameraService::disconnect()
{
    emit stateChanged(StreamState::Disconnected);
}

}
