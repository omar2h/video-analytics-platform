#include <vap/camera/services/camera_application_service.hpp>

#include <QUuid>

#include <vap/camera/repositories/i_camera_repository.hpp>
#include <vap/camera/validation/i_camera_validator.hpp>

namespace vap
{

CameraApplicationService::CameraApplicationService(ICameraRepository* repository, ICameraValidator* validator)
    : m_repository(repository), m_validator(validator)
{
}

QList<Camera> CameraApplicationService::cameras() const
{
    return m_repository->cameras();
}

CameraValidationResult CameraApplicationService::addCamera(const QString& name, const CameraConfig& config)
{
    const auto validation = m_validator->validate(name, config);
    if(validation.error != CameraValidationError::None)
        return validation;

    Camera camera;

    camera.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    camera.name = name;
    camera.config = config;

    m_repository->addCamera(camera);

    return {};
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
