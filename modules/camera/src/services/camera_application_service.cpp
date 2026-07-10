#include <vap/camera/services/camera_application_service.hpp>

#include <QUuid>

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

void CameraApplicationService::addCamera(const QString& name, const CameraConfig& config)
{
    Camera camera;

    camera.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    camera.name = name;
    camera.config = config;

    m_repository->addCamera(camera);
}

bool CameraApplicationService::updateCamera(
    const CameraId& cameraId,
    const QString& name,
    const CameraConfig& config)
{
    Camera camera(cameraId, name, config);
    return m_repository->updateCamera(camera);
}

void CameraApplicationService::removeCamera(const CameraId &cameraId)
{
    m_repository->removeCamera(cameraId);
}

}
