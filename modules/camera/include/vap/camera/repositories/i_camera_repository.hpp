#pragma once

#include <QList>

#include <vap/camera/camera.hpp>

namespace vap
{

class ICameraRepository
{
public:
    virtual ~ICameraRepository() = default;

    virtual QList<vap::Camera> cameras() const = 0;
};

}
