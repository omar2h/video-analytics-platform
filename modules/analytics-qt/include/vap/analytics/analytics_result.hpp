#pragma once

#include <vap/analytics/analytics_frame.hpp>
#include <vap/analytics/domain/detection.hpp>

#include <QSize>

#include <chrono>
#include <vector>

namespace vap
{

struct AnalyticsResult
{
    AnalyticsFrameIdentity identity;

    std::chrono::steady_clock::time_point decodeTime{};

    QSize imageSize;

    std::vector<Detection> detections;

    std::chrono::nanoseconds inferenceDuration{};
};

} // namespace vap
