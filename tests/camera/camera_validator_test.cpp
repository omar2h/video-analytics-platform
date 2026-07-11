#include <gtest/gtest.h>

#include <vap/camera/validation/camera_validator.hpp>

namespace vap
{

class CameraValidatorTest : public ::testing::Test
{
protected:
    CameraValidator validator;
    CameraConfig config {
        .url = "rtsp://192.168.1.10/live"
    };
};

TEST_F(CameraValidatorTest, EmptyNameReturnsEmptyName)
{
    const auto result = validator.validate("", config);

    EXPECT_EQ(result.error, CameraValidationError::EmptyName);
}

TEST_F(CameraValidatorTest, WhitespaceNameReturnsEmptyName)
{
    const auto result = validator.validate("    ", config);

    EXPECT_EQ(result.error, CameraValidationError::EmptyName);
}

TEST_F(CameraValidatorTest, ValidNameReturnsSuccess)
{
    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::None);
}

}
