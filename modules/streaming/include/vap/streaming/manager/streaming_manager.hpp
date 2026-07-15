#pragma once

#include <memory>

namespace vap
{
class StreamingSession;
class StreamingManager
{
public:
    StreamingManager();
    StreamingSession* session() const noexcept;
private:
    std::unique_ptr<StreamingSession> m_session;
};
}
