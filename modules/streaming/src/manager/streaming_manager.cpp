#include <vap/streaming/manager/streaming_manager.hpp>

#include <QDebug>

#include <vap/streaming/session/streaming_session.hpp>
#include <vap/camera/camera.hpp>

namespace vap
{
StreamingManager::StreamingManager() = default;

void StreamingManager::createSession(const QString &cameraId)
{
    if(contains(cameraId))
        return;
    qDebug() << "Creating session:" << cameraId;
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

void StreamingManager::startStreaming(const Camera& camera)
{
    auto* session = this->session(camera.id);

    Q_ASSERT(session);

    session->start(camera.config);
}

void StreamingManager::stopStreaming(const QString& cameraId)
{
    auto* s = session(cameraId);

    if (!s)
        return;

    s->stop();
}

StreamingSession* StreamingManager::session(const QString& cameraId) const
{
    auto it = m_sessions.find(cameraId);

    if (it == m_sessions.end())
        return nullptr;

    return it->second.get();
}
}
