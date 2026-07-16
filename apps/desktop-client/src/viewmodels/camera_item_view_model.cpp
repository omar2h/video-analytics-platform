#include "camera_item_view_model.hpp"

#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{
CameraItemViewModel::CameraItemViewModel(Camera camera, StreamingSession *session, QObject *parent)
    : QObject(parent),
    m_streamingSession(session),
    m_camera(std::move(camera))
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
        &CameraItemViewModel::onFrameReady);
}

int CameraItemViewModel::state() const
{
    return static_cast<int>(m_state);
}

QImage CameraItemViewModel::currentFrame() const
{
    return m_currentFrame;
}

int CameraItemViewModel::frameRevision() const
{
    return m_frameRevision;
}

QString CameraItemViewModel::id() const
{
    return m_camera.id;
}

QString CameraItemViewModel::name() const
{
    return m_camera.name;
}

QString CameraItemViewModel::url() const
{
    return m_camera.config.url;
}

void CameraItemViewModel::connectCamera()
{
    m_streamingSession->start(m_camera.config);
}

void CameraItemViewModel::disconnectCamera()
{
    m_streamingSession->stop();
}

void CameraItemViewModel::onFrameReady(const QImage& frame)
{
    m_currentFrame = frame;
    emit currentFrameChanged();

    ++m_frameRevision;
    emit frameRevisionChanged();
}

}
