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

TEST_F(CameraValidatorTest, ValidCameraReturnsSuccess)
{
    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::None);
}

TEST_F(CameraValidatorTest, EmptyUrlReturnsEmptyUrl)
{
    config.url = "";

    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::EmptyUrl);
}

TEST_F(CameraValidatorTest, WhitespaceUrlReturnsEmptyUrl)
{
    config.url = "    ";

    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::EmptyUrl);
}

TEST_F(CameraValidatorTest, InvalidSchemeReturnsInvalidUrlScheme)
{
    config.url = "http://192.168.1.10/live";

    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::InvalidUrlScheme);
}

TEST_F(CameraValidatorTest, RtspUrlReturnsSuccess)
{
    config.url = "rtsp://192.168.1.10/live";

    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::None);
}

TEST_F(CameraValidatorTest, FileUrlReturnsSuccess)
{
    config.url = "file:///home/omar/video.mp4";

    const auto result = validator.validate("Lobby Camera", config);

    EXPECT_EQ(result.error, CameraValidationError::None);
}

}
