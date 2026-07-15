#pragma once

#include <chrono>

namespace vap
{

struct ReconnectPolicy
{
    std::chrono::milliseconds initialDelay{1s};
    std::chrono::milliseconds maximumDelay{30s};
    double backoffMultiplier{2.0};
    std::optional<int> maximumRetries{std::nullopt};
};

}
