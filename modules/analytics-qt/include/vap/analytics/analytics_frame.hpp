#pragma once

#include <QImage>

#include <chrono>
#include <cstdint>

namespace vap
{

struct AnalyticsFrameIdentity
{
    std::uint64_t streamRunId{};
    std::uint64_t analyticsGeneration{};
    std::uint64_t frameRevision{};

    bool operator==(const AnalyticsFrameIdentity&) const = default;
};

struct AnalyticsFrame
{
    AnalyticsFrameIdentity identity;

    std::chrono::steady_clock::time_point decodeTime{};

    // Owning image value. Pixels are read-only after submission.
    // Expected format: QImage::Format_RGB888.
    QImage image;
};

} // namespace vap
