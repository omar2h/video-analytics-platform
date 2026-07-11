#pragma once

namespace vap
{

enum class CameraValidationError
{
    None,
    EmptyName,
    EmptyUrl,
    InvalidUrlScheme
};

struct CameraValidationResult
{
    CameraValidationError error{
        CameraValidationError::None
    };
};

}
