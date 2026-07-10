#pragma once

#include <QObject>
#include <vap/camera/camera.hpp>

namespace vap
{

using CameraId = QString;

class ICameraApplicationService
{
public:
    virtual ~ICameraApplicationService() = default;
    virtual QList<Camera> cameras() const = 0;

    virtual void addCamera(const QString& name, const CameraConfig& config) = 0;

    virtual bool updateCamera(const CameraId& cameraId, const QString& name, const CameraConfig& config) = 0;

    virtual void removeCamera(const CameraId& cameraId) = 0;
};
}
