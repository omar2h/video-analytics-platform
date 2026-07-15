#include <vap/streaming/reconnect/reconnect_policy.hpp>

namespace vap
{

ReconnectPolicy::ReconnectPolicy(
    int maxRetries,
    std::chrono::milliseconds retryDelay)
    : m_maxRetries(maxRetries)
    , m_retryDelay(retryDelay)
{
}

bool ReconnectPolicy::shouldRetry() const
{
    return m_retryCount < m_maxRetries;
}

std::chrono::milliseconds ReconnectPolicy::retryDelay() const
{
    return m_retryDelay;
}

void ReconnectPolicy::recordRetry()
{
    ++m_retryCount;
}

void ReconnectPolicy::reset()
{
    m_retryCount = 0;
}

} // namespace vap
