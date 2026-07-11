#include <vap/camera/validation/camera_validator.hpp>

namespace vap
{

CameraValidationResult CameraValidator::validate(const QString& cameraName, const CameraConfig& config) const
{
    Q_UNUSED(config)

    const auto name = cameraName.trimmed();
    const auto url = config.url.trimmed();

    if(name.isEmpty())
        return {
            .error = CameraValidationError::EmptyName
        };

    if(url.isEmpty())
        return {
            .error = CameraValidationError::EmptyUrl
        };

    const bool validScheme =
        url.startsWith("rtsp://") ||
        url.startsWith("file://");

    if(!validScheme)
        return {
            .error = CameraValidationError::InvalidUrlScheme
        };

    return CameraValidationResult{};
}

}
