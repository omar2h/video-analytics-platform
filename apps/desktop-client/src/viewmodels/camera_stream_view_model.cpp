#include "camera_stream_view_model.hpp"

#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{
CameraStreamViewModel::CameraStreamViewModel(const QString& cameraId, StreamingSession *session, QObject *parent)
    : QObject(parent),
    m_cameraId(cameraId),
    m_streamingSession(session)
{
    Q_ASSERT(m_streamingSession);
    connect(m_streamingSession,
            &StreamingSession::stateChanged,
            this,
            [this](const ConnectionState& state)
            {
                m_state = state;
                emit stateChanged();
            });
    connect(
        m_streamingSession,
        &StreamingSession::frameReady,
        this,
        &CameraStreamViewModel::onFrameReady);
}

int CameraStreamViewModel::state() const
{
    return static_cast<int>(m_state);
}

QImage CameraStreamViewModel::currentFrame() const
{
    return m_currentFrame;
}

int CameraStreamViewModel::frameRevision() const
{
    return m_frameRevision;
}

void CameraStreamViewModel::onFrameReady(const QImage& frame)
{
    m_currentFrame = frame;
    emit currentFrameChanged();

    ++m_frameRevision;
    emit frameRevisionChanged();
}

}
