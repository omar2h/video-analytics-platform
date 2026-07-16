#include "camera_view_model.hpp"

#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{

CameraViewModel::CameraViewModel(StreamingSession* streamingSession, QObject* parent)
    : QObject(parent), m_streamingSession(streamingSession)
{
    Q_ASSERT(m_streamingSession != nullptr);
    connect(m_streamingSession,
            &StreamingSession::stateChanged,
            this,
            [this](ConnectionState state) {
                m_state = state;
                emit stateChanged();
                emit stateTextChanged();
            });

    connect(m_streamingSession,
            &StreamingSession::frameReady,
            this,
            &CameraViewModel::onFrameReady);
}

QString CameraViewModel::stateText() const
{
    switch (m_state)
    {
    case ConnectionState::Disconnected:
        return "Disconnected";

    case ConnectionState::Connecting:
        return "Connecting";

    case ConnectionState::Connected:
        return "Connected";

    case ConnectionState::Reconnecting:
        return "Reconnecting";

    case ConnectionState::Error:
        return "Error";
    }

    return "Unknown";
}

int CameraViewModel::state() const
{
    return static_cast<int>(m_state);
}

QImage CameraViewModel::currentFrame() const
{
    return m_currentFrame;
}

int CameraViewModel::frameRevision() const
{
    return m_frameRevision;
}

void CameraViewModel::onFrameReady(const QImage& frame)
{
    m_currentFrame = frame;
    emit currentFrameChanged();

    ++m_frameRevision;
    emit frameRevisionChanged();
}

}

