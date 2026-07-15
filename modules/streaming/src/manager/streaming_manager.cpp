#include <vap/streaming/manager/streaming_manager.hpp>

#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{
StreamingManager::StreamingManager()
{
    m_session = std::make_unique<StreamingSession>();
}

StreamingSession *StreamingManager::session() const noexcept
{
    return m_session.get();
}
}
