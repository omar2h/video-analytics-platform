#include <vap/streaming/frame/frame_exchange.hpp>

#include <mutex>

namespace vap
{

void FrameExchange::publish(QImage frame, std::chrono::steady_clock::time_point decodeTime)
{
    std::lock_guard<QMutex> lock(m_mutex);
    m_snapshot.image = std::move(frame);
    ++m_snapshot.revision;
    m_snapshot.decodeTime = decodeTime;
    m_snapshot.valid = true;
}

FrameSnapshot FrameExchange::snapshot() const
{
    std::lock_guard<QMutex> lock(m_mutex);
    return m_snapshot;
}

}
