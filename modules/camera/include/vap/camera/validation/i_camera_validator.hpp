#pragma once

#include <vap/camera/validation/camera_validation.hpp>
#include <vap/camera/camera_config.hpp>

namespace vap
{

class ICameraValidator
{
public:
    virtual ~ICameraValidator() = default;

    virtual CameraValidationResult validate(const QString& name, const CameraConfig& config) const = 0;
};
}
