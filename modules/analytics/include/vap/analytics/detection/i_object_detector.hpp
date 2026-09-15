#pragma once

#include <string_view>
#include <vector>

#include <vap/analytics/domain/detection.hpp>

namespace cv
{
class Mat;
}

namespace vap
{

class IObjectDetector
{
public:
    virtual ~IObjectDetector() = default;

    // Input: nonempty CV_8UC3 image in BGR channel order.
    // The caller keeps the pixels alive and unmodified until return.
    // The detector does not retain the image.
    //
    // Returns independent detection values.
    // An empty vector means no objects passed filtering.
    // Invalid input or inference failure throws an exception.
    //
    // Call serially: one detection operation at a time per instance.
    [[nodiscard]]
    virtual std::vector<Detection> detect(const cv::Mat& image) = 0;

    // Returns the class label, or "unknown" for an invalid ID.
    // The returned text remains valid while the detector is alive.
    [[nodiscard]]
    virtual std::string_view className(int classId) const noexcept = 0;
};

} // namespace vap
