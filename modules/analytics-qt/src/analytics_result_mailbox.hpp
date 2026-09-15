#pragma once

#include <vap/analytics/analytics_result.hpp>

#include <mutex>
#include <optional>
#include <utility>

namespace vap::detail
{

class AnalyticsResultMailbox
{
public:
    // Returns true when the caller must queue a GUI notification.
    [[nodiscard]]
    bool publish(AnalyticsResult result)
    {
        std::lock_guard lock(m_mutex);

        m_latest = std::move(result);

        if (m_notificationPending)
            return false;

        m_notificationPending = true;
        return true;
    }

    // Called only when the queued GUI notification executes.
    // Returns whether there is a result worth announcing.
    [[nodiscard]]
    bool acknowledgeNotification()
    {
        std::lock_guard lock(m_mutex);

        m_notificationPending = false;
        return m_latest.has_value();
    }

    [[nodiscard]]
    std::optional<AnalyticsResult> takeLatest()
    {
        std::lock_guard lock(m_mutex);

        auto result = std::move(m_latest);
        m_latest.reset();

        return result;
    }

    void clear()
    {
        std::lock_guard lock(m_mutex);
        m_latest.reset();

        // A previously queued notification may still exist.
        // Leave m_notificationPending unchanged.
    }

private:
    std::mutex m_mutex;
    std::optional<AnalyticsResult> m_latest;
    bool m_notificationPending{false};
};

} // namespace vap::detail
