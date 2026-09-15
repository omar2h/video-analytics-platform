#include <gtest/gtest.h>

#include "yolox_postprocessing.hpp"
#include "yolox_model_spec.hpp"

#include <vector>

namespace vap
{

TEST(YoloXPostprocessingTest, IdenticalBoxesHaveIoUOfOne)
{
    const BoundingBox box{
        0.0, 0.0, 100.0, 100.0
    };

    EXPECT_DOUBLE_EQ(
        detail::intersectionOverUnion(box, box),
        1.0);
}

TEST(YoloXPostprocessingTest, SeparateBoxesHaveIoUOfZero)
{
    const BoundingBox first{
        0.0, 0.0, 100.0, 100.0
    };

    const BoundingBox second{
        200.0, 0.0, 100.0, 100.0
    };

    EXPECT_DOUBLE_EQ(
        detail::intersectionOverUnion(first, second),
        0.0);
}

TEST(YoloXPostprocessingTest, PartialOverlapHasExpectedIoU)
{
    const BoundingBox first{
        0.0, 0.0, 100.0, 100.0
    };

    const BoundingBox second{
        50.0, 0.0, 100.0, 100.0
    };

    EXPECT_NEAR(
        detail::intersectionOverUnion(first, second),
        1.0 / 3.0,
        1e-9);
}

TEST(YoloXPostprocessingTest, NmsKeepsStrongerSameClassDetection)
{
    const std::vector<Detection> detections{
        {{0.0, 0.0, 100.0, 100.0}, 2, 0.70f},
        {{0.0, 0.0, 100.0, 100.0}, 2, 0.90f}
    };

    const auto result = detail::applyNms(detections, 0.45);

    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].classId, 2);
    EXPECT_FLOAT_EQ(result[0].score, 0.90f);
}

TEST(YoloXPostprocessingTest, NmsKeepsOverlappingDifferentClasses)
{
    const std::vector<Detection> detections{
        {{0.0, 0.0, 100.0, 100.0}, 2, 0.90f},
        {{0.0, 0.0, 100.0, 100.0}, 0, 0.80f}
    };

    const auto result = detail::applyNms(detections, 0.45);

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].classId, 2);
    EXPECT_EQ(result[1].classId, 0);
}

TEST(YoloXPostprocessingTest, DecoderFiltersLowConfidencePredictions)
{
    std::vector<float> predictions(
        detail::CandidateCount * detail::ValuesPerCandidate,
        0.0f
        );

    // First candidate: objectness × class probability.
    predictions[4] = 0.50f;
    predictions[5 + 2] = 0.80f; // Class 2: car.

    const auto filtered = detail::decodeDetections(
        predictions,
        1.0,  // Image scale.
        0.50f // Confidence threshold.
        );

    EXPECT_TRUE(filtered.empty());

    const auto accepted = detail::decodeDetections(
        predictions,
        1.0,
        0.30f
        );

    ASSERT_EQ(accepted.size(), 1u);
    EXPECT_EQ(accepted[0].classId, 2);
    EXPECT_FLOAT_EQ(accepted[0].score, 0.40f);
}

TEST(YoloXPostprocessingTest, DecoderScalesBoxToOriginalImage)
{
    std::vector<float> predictions(
        detail::CandidateCount * detail::ValuesPerCandidate,
        0.0f
        );

    // First candidate: grid position (0, 0), stride 8.
    predictions[0] = 2.0f; // Encoded center X.
    predictions[1] = 3.0f; // Encoded center Y.
    predictions[2] = 0.0f; // Encoded width.
    predictions[3] = 0.0f; // Encoded height.
    predictions[4] = 1.0f; // Objectness.
    predictions[5 + 2] = 0.90f; // Car probability.

    const auto result = detail::decodeDetections(
        predictions,
        0.5,  // Original image was resized to half its size.
        0.30f
        );

    ASSERT_EQ(result.size(), 1u);

    const auto& box = result[0].box;

    EXPECT_DOUBLE_EQ(box.x, 24.0);
    EXPECT_DOUBLE_EQ(box.y, 40.0);
    EXPECT_DOUBLE_EQ(box.width, 16.0);
    EXPECT_DOUBLE_EQ(box.height, 16.0);
}

} // namespace vap
