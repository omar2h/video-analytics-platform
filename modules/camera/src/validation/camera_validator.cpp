#include <vap/camera/validation/camera_validator.hpp>

namespace vap
{

CameraValidationResult CameraValidator::validate(const QString& name, const CameraConfig& config) const
{
    Q_UNUSED(config)

    if(name.trimmed().isEmpty())
        return {
            .error = CameraValidationError::EmptyName
        };

    return CameraValidationResult{};
}

}
