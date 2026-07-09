#pragma once

#include <vap/camera/repositories/i_camera_repository.hpp>

namespace vap
{

class InMemoryCameraRepository : public ICameraRepository
{
public:
    InMemoryCameraRepository();

    QList<Camera> cameras() const override;

    void addCamera(const Camera& camera) override;

    bool updateCamera(const Camera& camera) override;

    void removeCamera(const CameraId& cameraId) override;

private:
    QList<Camera> m_cameras;
};

}
