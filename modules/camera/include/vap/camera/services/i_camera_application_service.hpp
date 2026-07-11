#pragma once

#include <QObject>
#include <vap/camera/camera.hpp>
#include <vap/camera/validation/camera_validation.hpp>

namespace vap
{

using CameraId = QString;

class ICameraApplicationService
{
public:
    virtual ~ICameraApplicationService() = default;
    virtual QList<Camera> cameras() const = 0;

    virtual CameraValidationResult addCamera(const QString& name, const CameraConfig& config) = 0;

    virtual CameraValidationResult updateCamera(const CameraId& cameraId, const QString& name, const CameraConfig& config) = 0;

    virtual void removeCamera(const CameraId& cameraId) = 0;
};
}
