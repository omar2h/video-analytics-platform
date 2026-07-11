#pragma once

#include <vap/camera/validation/i_camera_validator.hpp>

namespace vap
{

class CameraValidator : public ICameraValidator
{
public:
    CameraValidationResult validate(const QString& name, const CameraConfig& config) const override;
};

}
