#pragma once

#include <QList>

#include <vap/camera/camera.hpp>

using CameraId = QString;

namespace vap
{

class ICameraRepository
{
public:
    virtual ~ICameraRepository() = default;

    virtual QList<vap::Camera> cameras() const = 0;

    virtual void addCamera(const Camera& camera) = 0;

    virtual bool updateCamera(const Camera& camera) = 0;

    virtual void removeCamera(const CameraId& cameraId) = 0;
};

}
