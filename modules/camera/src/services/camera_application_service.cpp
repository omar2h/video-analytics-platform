#include <vap/camera/services/camera_application_service.hpp>

#include <vap/camera/repositories/i_camera_repository.hpp>

namespace vap
{

CameraApplicationService::CameraApplicationService(ICameraRepository* repository) : m_repository(repository)
{
}

QList<Camera> CameraApplicationService::cameras() const
{
    return m_repository->cameras();
}

void CameraApplicationService::addCamera(const Camera &camera)
{
    m_repository->addCamera(camera);
}

bool CameraApplicationService::updateCamera(const Camera &camera)
{
    return m_repository->updateCamera(camera);
}

void CameraApplicationService::removeCamera(const CameraId &cameraId)
{
    m_repository->removeCamera(cameraId);
}

}
