#include "camera_stream_view_model.hpp"

#include <QDebug>

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
            &CameraStreamViewModel::onStateChanged);

    connect(
        m_streamingSession,
        &StreamingSession::frameReady,
        this,
        &CameraStreamViewModel::onFrameReady);

    connect(
        m_streamingSession,
        &StreamingSession::statisticsUpdated,
        this,
        &CameraStreamViewModel::onStatisticsUpdated);
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

bool CameraStreamViewModel::hasVideo() const
{
    return m_hasVideo;
}

QString CameraStreamViewModel::codec() const
{
    return m_streamingSession->statistics().codec;
}

QString CameraStreamViewModel::resolution() const
{
    const auto& stats = m_streamingSession->statistics();

    return QString("%1 × %2")
        .arg(stats.resolution.width())
        .arg(stats.resolution.height());
}

quint64 CameraStreamViewModel::framesDecoded() const
{
    return m_streamingSession->statistics().framesDecoded;
}

quint64 CameraStreamViewModel::packetsReceived() const
{
    return m_streamingSession->statistics().packetsReceived;
}

double CameraStreamViewModel::fps() const
{
    return m_streamingSession->statistics().fps;
}

double CameraStreamViewModel::bitrateMbps() const
{
    return m_streamingSession->statistics().bitrateMbps;
}

void CameraStreamViewModel::onFrameReady(const QImage& frame)
{
    if (!m_hasVideo)
    {
        m_hasVideo = true;
        emit hasVideoChanged();
    }

    m_currentFrame = frame;
    emit currentFrameChanged();

    ++m_frameRevision;
    emit frameRevisionChanged();
}

void CameraStreamViewModel::onStateChanged(ConnectionState state)
{
    m_state = state;

    switch (state)
    {
    case ConnectionState::Connecting:
    case ConnectionState::Disconnected:
        m_currentFrame = {};
        emit currentFrameChanged();

        if (m_hasVideo)
        {
            m_hasVideo = false;
            emit hasVideoChanged();
        }
        break;

    case ConnectionState::Connected:
        break;

    case ConnectionState::Reconnecting:
        break;
    }

    emit stateChanged();
}

void CameraStreamViewModel::onStatisticsUpdated()
{
    emit statisticsChanged();
}

}
