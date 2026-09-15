#pragma once

#include <vap/analytics/analytics_frame.hpp>
#include <vap/analytics/analytics_result.hpp>
#include <vap/analytics/detection/i_object_detector.hpp>

namespace vap::detail
{

[[nodiscard]]
AnalyticsResult processAnalyticsFrame(
    const AnalyticsFrame& frame,
    IObjectDetector& detector);

} // namespace vap::detail
