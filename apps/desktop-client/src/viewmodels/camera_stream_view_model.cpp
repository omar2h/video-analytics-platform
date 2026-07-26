#include "camera_stream_view_model.hpp"

#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <vap/streaming/session/streaming_session.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

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
        &StreamingSession::frameUpdated,
        this,
        &CameraStreamViewModel::onFrameUpdated);

    connect(
        m_streamingSession,
        &StreamingSession::statisticsUpdated,
        this,
        &CameraStreamViewModel::onStatisticsUpdated);

    connect(
        m_streamingSession,
        &StreamingSession::recordingStateChanged,
        this,
        &CameraStreamViewModel::onRecordingStateChanged);

    connect(
        m_streamingSession,
        &StreamingSession::recordingDurationChanged,
        this,
        &CameraStreamViewModel::onRecordingDurationChanged);
}

int CameraStreamViewModel::state() const
{
    return static_cast<int>(m_state);
}

QImage CameraStreamViewModel::currentFrame() const
{
    return m_currentFrame;
}

void CameraStreamViewModel::setCurrentFrame(QImage image)
{
    m_currentFrame = std::move(image);
    emit currentFrameChanged();
}

quint64 CameraStreamViewModel::frameRevision() const
{
    return m_frameRevision;
}

void CameraStreamViewModel::setFrameRevision(quint64 frameRevision)
{
    m_frameRevision = frameRevision;
    emit frameRevisionChanged();
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

bool CameraStreamViewModel::recording() const
{
    return m_recordingState == RecordingState::Recording;
}

QString CameraStreamViewModel::recordingStateText() const
{
    switch (m_recordingState)
    {
    case RecordingState::Stopped:
        return "Idle";

    case RecordingState::Recording:
        return "Recording";

    case RecordingState::Stopping:
        return "Stopping...";
    }

    return {};
}

bool CameraStreamViewModel::recordingActionEnabled() const
{
    return m_state == ConnectionState::Connected;
}

QString CameraStreamViewModel::recordingDurationText() const
{
    const qint64 hours = m_recordingDuration / 3600;
    const qint64 minutes = (m_recordingDuration % 3600) / 60;
    const qint64 seconds = m_recordingDuration % 60;

    return QString("%1:%2:%3")
        .arg(hours,   2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));

}

QString CameraStreamViewModel::recordingFileName() const
{
    return m_recordingFileName;
}

void CameraStreamViewModel::startRecording()
{
    RecordingConfiguration config;

    // temporary filename
    QDir videosDir(
        QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));

    videosDir.mkpath("VAP");
    config.outputPath =
        videosDir.filePath(
            QString("%1_%2.mp4")
                .arg(m_streamingSession->cameraName())
                .arg(QDateTime::currentDateTime()
                         .toString("yyyyMMdd_HHmmss")));

    m_recordingFileName = QFileInfo(config.outputPath).fileName();
    m_streamingSession->startRecording(config);
}

void CameraStreamViewModel::stopRecording()
{
    m_streamingSession->stopRecording();
}

double CameraStreamViewModel::fps() const
{
    return m_streamingSession->statistics().fps;
}

double CameraStreamViewModel::bitrateMbps() const
{
    return m_streamingSession->statistics().bitrateMbps;
}

void CameraStreamViewModel::onFrameUpdated()
{
    auto snapshot = m_streamingSession->currentFrame();

    if (!snapshot.valid)
        return;

    setCurrentFrame(std::move(snapshot.image));
    setFrameRevision(snapshot.revision);

    if (!m_hasVideo)
    {
        m_hasVideo = true;
        emit hasVideoChanged();
    }
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
    emit recordingActionEnabledChanged();
}

void CameraStreamViewModel::onStatisticsUpdated()
{
    emit statisticsChanged();
}

void CameraStreamViewModel::onRecordingStateChanged(RecordingState state)
{
    if (m_recordingState == state)
        return;

    m_recordingState = state;

    emit recordingChanged();
    emit recordingActionEnabledChanged();
}

void CameraStreamViewModel::onRecordingDurationChanged(qint64 seconds)
{
    if (m_recordingDuration == seconds)
        return;

    m_recordingDuration = seconds;

    emit recordingDurationChanged();
}

}
