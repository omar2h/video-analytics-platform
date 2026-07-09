#pragma once

#include <QObject>
#include <vap/camera/camera.hpp>

namespace vap
{

class ICameraApplicationService
{
public:
    virtual ~ICameraApplicationService() = default;
    virtual QList<Camera> cameras() const = 0;
};
}
