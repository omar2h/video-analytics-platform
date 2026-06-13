#pragma once

#include <vap/camera/camera_config.hpp>

namespace vap
{

struct Camera
{
    QString id;
    QString name;
    CameraConfig config;
};

}
