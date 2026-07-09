#pragma once

#include <QObject>
#include <vap/camera/camera.hpp>

using CameraId = QString;

namespace vap
{

class ICameraApplicationService
{
public:
    virtual ~ICameraApplicationService() = default;
    virtual QList<Camera> cameras() const = 0;

    virtual void addCamera(const Camera& camera) = 0;

    virtual bool updateCamera(const Camera& camera) = 0;

    virtual void removeCamera(const CameraId& cameraId) = 0;
};
}
