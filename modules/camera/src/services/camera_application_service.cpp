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

}
