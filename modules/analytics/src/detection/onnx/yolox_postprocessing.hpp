#pragma once

#include <span>
#include <vector>

#include <vap/analytics/domain/detection.hpp>

namespace vap::detail
{

[[nodiscard]]
std::vector<Detection> decodeDetections(
    std::span<const float> predictions,
    double imageScale,
    float scoreThreshold);

[[nodiscard]]
double intersectionOverUnion(
    const BoundingBox& first,
    const BoundingBox& second) noexcept;

[[nodiscard]]
std::vector<Detection> applyNms(
    std::vector<Detection> detections,
    double overlapThreshold);

} // namespace vap::detail
