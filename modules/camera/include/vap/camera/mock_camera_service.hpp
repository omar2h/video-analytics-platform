#pragma once

#include <vap/camera/i_camera_service.hpp>

namespace vap
{

class MockCameraService final : public ICameraService
{
    Q_OBJECT

public:
    explicit MockCameraService(QObject* parent = nullptr);

    void connect(const CameraConfig& config) override;

    void disconnect() override;
};

}
