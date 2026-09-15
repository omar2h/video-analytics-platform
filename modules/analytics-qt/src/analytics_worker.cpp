#include "analytics_worker.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <chrono>
#include <stdexcept>
#include <utility>

namespace vap::detail
{

AnalyticsResult processAnalyticsFrame(
    const AnalyticsFrame& frame,
    IObjectDetector& detector)
{
    if (frame.image.isNull())
    {
        throw std::invalid_argument(
            "Analytics received an empty image.");
    }

    if (frame.image.format() != QImage::Format_RGB888)
    {
        throw std::invalid_argument(
            "Analytics requires an RGB888 image.");
    }

    // OpenCV's external-buffer constructor requires a mutable
    // pointer. This view is used only as a read-only input.
    const cv::Mat rgb(
        frame.image.height(),
        frame.image.width(),
        CV_8UC3,
        const_cast<uchar*>(frame.image.constBits()),
        static_cast<std::size_t>(frame.image.bytesPerLine()));

    cv::Mat bgr;
    cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);

    const auto started = std::chrono::steady_clock::now();

    auto detections = detector.detect(bgr);

    const auto finished = std::chrono::steady_clock::now();

    AnalyticsResult result;
    result.identity = frame.identity;
    result.decodeTime = frame.decodeTime;
    result.imageSize = frame.image.size();
    result.detections = std::move(detections);
    result.inferenceDuration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            finished - started);

    return result;
}

} // namespace vap::detail
