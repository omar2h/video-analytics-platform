#include "yolox_postprocessing.hpp"
#include "yolox_model_spec.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace vap::detail
{

std::vector<Detection> decodeDetections(
    std::span<const float> predictions,
    double imageScale,
    float scoreThreshold)
{
    if (predictions.size() != CandidateCount * ValuesPerCandidate)
    {
        throw std::invalid_argument(
            "Unexpected YOLOX prediction buffer size.");
    }

    if (!std::isfinite(imageScale) || imageScale <= 0.0)
    {
        throw std::invalid_argument("Invalid image scale.");
    }

    if (!std::isfinite(scoreThreshold) ||
        scoreThreshold < 0.0F || scoreThreshold > 1.0F)
    {
        throw std::invalid_argument(
            "Score threshold must be between 0 and 1.");
    }

    std::vector<Detection> detections;
    std::size_t candidateIndex = 0;

    for (const int stride : Strides)
    {
        const int gridWidth = InputWidth / stride;
        const int gridHeight = InputHeight / stride;

        for (int gridY = 0; gridY < gridHeight; ++gridY)
        {
            for (int gridX = 0; gridX < gridWidth;
                 ++gridX, ++candidateIndex)
            {
                const auto row = predictions.subspan(
                    candidateIndex * ValuesPerCandidate,
                    ValuesPerCandidate);

                int bestClassId = -1;
                float bestScore = 0.0F;

                for (std::size_t classId = 0;
                     classId < ClassCount;
                     ++classId)
                {
                    const float score = row[4] * row[5 + classId];

                    if (std::isfinite(score) && score > bestScore)
                    {
                        bestScore = score;
                        bestClassId = static_cast<int>(classId);
                    }
                }

                if (bestClassId < 0 || bestScore < scoreThreshold)
                    continue;

                const double centerX =
                    (static_cast<double>(row[0]) + gridX) * stride;

                const double centerY =
                    (static_cast<double>(row[1]) + gridY) * stride;

                const double width =
                    std::exp(static_cast<double>(row[2])) * stride;

                const double height =
                    std::exp(static_cast<double>(row[3])) * stride;

                const BoundingBox box{
                    (centerX - width / 2.0) / imageScale,
                    (centerY - height / 2.0) / imageScale,
                    width / imageScale,
                    height / imageScale
                };

                if (!std::isfinite(box.x) ||
                    !std::isfinite(box.y) ||
                    !std::isfinite(box.width) ||
                    !std::isfinite(box.height) ||
                    !std::isfinite(box.x + box.width) ||
                    !std::isfinite(box.y + box.height) ||
                    !std::isfinite(box.width * box.height) ||
                    box.width <= 0.0 ||
                    box.height <= 0.0)
                {
                    continue;
                }

                detections.push_back(
                    Detection{box, bestClassId, bestScore});
            }
        }
    }

    return detections;
}

double intersectionOverUnion(
    const BoundingBox& first,
    const BoundingBox& second) noexcept
{
    const double left = std::max(first.x, second.x);
    const double top = std::max(first.y, second.y);

    const double right = std::min(
        first.x + first.width,
        second.x + second.width);

    const double bottom = std::min(
        first.y + first.height,
        second.y + second.height);

    const double intersectionWidth =
        std::max(0.0, right - left);

    const double intersectionHeight =
        std::max(0.0, bottom - top);

    const double intersectionArea =
        intersectionWidth * intersectionHeight;

    const double firstArea = first.width * first.height;
    const double secondArea = second.width * second.height;

    const double unionArea =
        firstArea + secondArea - intersectionArea;

    return unionArea > 0.0
        ? intersectionArea / unionArea
        : 0.0;
}

std::vector<Detection> applyNms(
    std::vector<Detection> detections,
    double overlapThreshold)
{
    if (!std::isfinite(overlapThreshold) ||
        overlapThreshold < 0.0 ||
        overlapThreshold > 1.0)
    {
        throw std::invalid_argument(
            "NMS threshold must be between 0 and 1.");
    }

    // Process the strongest predictions first.
    std::stable_sort(
        detections.begin(),
        detections.end(),
        [](const Detection& first, const Detection& second)
        {
            return first.score > second.score;
        });

    std::vector<Detection> kept;
    kept.reserve(detections.size());

    for (const auto& candidate : detections)
    {
        const bool shouldSuppress = std::any_of(
            kept.begin(),
            kept.end(),
            [&](const Detection& accepted)
            {
                if (candidate.classId != accepted.classId)
                    return false;

                return intersectionOverUnion(
                    candidate.box,
                    accepted.box) > overlapThreshold;
            });

        if (!shouldSuppress)
            kept.push_back(candidate);
    }

    return kept;
}

} // namespace vap::detail
