#pragma once

#include <filesystem>
#include <memory>

#include <vap/analytics/detection/i_object_detector.hpp>

namespace vap
{

struct OnnxYoloXConfiguration
{
    std::filesystem::path modelPath;

    float scoreThreshold{0.30F};
    double nmsThreshold{0.45};

    int intraOpThreads{2};
};

// Supports the official raw-output YOLOX-Nano 416x416 model.
// Use one caller at a time per detector instance.
class OnnxYoloXDetector final : public IObjectDetector
{
public:
    explicit OnnxYoloXDetector(
        const OnnxYoloXConfiguration& configuration);

    ~OnnxYoloXDetector() override;

    OnnxYoloXDetector(const OnnxYoloXDetector&) = delete;
    OnnxYoloXDetector& operator=(const OnnxYoloXDetector&) = delete;

    OnnxYoloXDetector(OnnxYoloXDetector&&) = delete;
    OnnxYoloXDetector& operator=(OnnxYoloXDetector&&) = delete;

    [[nodiscard]]
    std::vector<Detection> detect(const cv::Mat& image) override;

    [[nodiscard]]
    std::string_view className(int classId) const noexcept override;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vap
