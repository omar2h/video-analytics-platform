#pragma once

#include <vap/camera/services/i_camera_application_service.hpp>

namespace vap
{

class ICameraRepository;
class ICameraValidator;
class StreamingManager;

class CameraApplicationService final : public ICameraApplicationService
{
public:
    explicit CameraApplicationService(ICameraRepository* repository, ICameraValidator* validator, StreamingManager* streamingManager);
    QList<Camera> cameras() const override;

    CameraValidationResult addCamera(const QString& name, const CameraConfig& config) override;

    CameraValidationResult updateCamera(
        const CameraId& cameraId,
        const QString& name,
        const CameraConfig& config);

    void removeCamera(const CameraId& cameraId) override;

private:
    ICameraRepository* m_repository{};
    ICameraValidator* m_validator{};
    StreamingManager* m_streamingManager;
};

}
