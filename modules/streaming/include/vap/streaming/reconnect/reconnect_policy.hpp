#pragma once

#include <chrono>

namespace vap
{

class ReconnectPolicy
{
public:
    explicit ReconnectPolicy(
        int maxRetries = 30,
        std::chrono::milliseconds retryDelay = std::chrono::seconds(1));

    bool shouldRetry() const;

    std::chrono::milliseconds retryDelay() const;

    void recordRetry();

    void reset();

private:
    int m_retryCount{0};
    int m_maxRetries;
    std::chrono::milliseconds m_retryDelay;
};

} // namespace vap
