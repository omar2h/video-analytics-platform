#include <vap/streaming/manager/streaming_manager.hpp>

#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{
StreamingManager::StreamingManager() = default;

void StreamingManager::createSession(const QString &cameraId)
{
    if(contains(cameraId))
        return;
    m_sessions.emplace(cameraId, std::make_unique<StreamingSession>());
}

void StreamingManager::removeSession(const QString &cameraId)
{
    m_sessions.erase(cameraId);
}

bool StreamingManager::contains(const QString& cameraId) const
{
    return m_sessions.contains(cameraId);
}

StreamingSession* StreamingManager::session(const QString& cameraId) const
{
    auto it = m_sessions.find(cameraId);

    if (it == m_sessions.end())
        return nullptr;

    return it->second.get();
}
}
