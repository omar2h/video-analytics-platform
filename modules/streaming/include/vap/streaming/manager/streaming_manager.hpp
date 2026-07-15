#pragma once

#include <memory>
#include <map>
#include <QString>

namespace vap
{
class StreamingSession;
class StreamingManager
{
public:
    StreamingManager();

    void createSession(const QString& cameraId);

    void removeSession(const QString& cameraId);

    StreamingSession* session(const QString& cameraId) const;

    bool contains(const QString& cameraId) const;
private:
    std::map<QString, std::unique_ptr<StreamingSession>> m_sessions;
};
}
