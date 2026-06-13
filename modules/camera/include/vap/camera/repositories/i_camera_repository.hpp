#pragma once

#include <QList>

#include <vap/camera/camera.hpp>

class ICameraRepository
{
public:
    virtual ~ICameraRepository() = default;

    virtual QList<vap::Camera> cameras() const = 0;
};
