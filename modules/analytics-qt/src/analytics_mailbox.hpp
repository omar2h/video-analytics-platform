#pragma once

#include <vap/analytics/analytics_frame.hpp>

#include <condition_variable>
#include <mutex>
#include <optional>
#include <stop_token>
#include <utility>

namespace vap::detail
{

class AnalyticsFrameMailbox
{
public:
    void submit(AnalyticsFrame frame)
    {
        {
            std::lock_guard lock(m_mutex);

            // Replace the older pending frame.
            m_pending = std::move(frame);
        }

        m_available.notify_one();
    }

    [[nodiscard]]
    std::optional<AnalyticsFrame> waitAndTake(
        std::stop_token stopToken)
    {
        std::unique_lock lock(m_mutex);

        m_available.wait(
            lock,
            stopToken,
            [this]
            {
                return m_pending.has_value();
            });

        // Prioritize shutdown over pending work.
        if (stopToken.stop_requested())
            return std::nullopt;

        auto frame = std::move(m_pending);
        m_pending.reset();

        return frame;
    }

    void clear()
    {
        std::lock_guard lock(m_mutex);
        m_pending.reset();
    }

private:
    std::mutex m_mutex;
    std::condition_variable_any m_available;
    std::optional<AnalyticsFrame> m_pending;
};

} // namespace vap::detail
