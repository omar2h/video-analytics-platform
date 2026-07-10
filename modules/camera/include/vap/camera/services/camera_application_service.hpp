#pragma once

#include <vap/camera/services/i_camera_application_service.hpp>

namespace vap
{

class ICameraRepository;

class CameraApplicationService final : public ICameraApplicationService
{
public:
    explicit CameraApplicationService(ICameraRepository* repository);
    QList<Camera> cameras() const override;

    void addCamera(const QString& name, const CameraConfig& config) override;

    bool updateCamera(
        const CameraId& cameraId,
        const QString& name,
        const CameraConfig& config);

    void removeCamera(const CameraId& cameraId) override;

private:
    ICameraRepository* m_repository{};
};

}
