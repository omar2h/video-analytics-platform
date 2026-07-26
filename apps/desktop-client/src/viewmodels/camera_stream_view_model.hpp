#pragma once

#include <QObject>
#include <QImage>

#include <vap/common/connection_state.hpp>
#include <vap/streaming/recording/recording_state.hpp>

namespace vap
{
class StreamingSession;
class CameraStreamViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int state
        READ state
        NOTIFY stateChanged)

    Q_PROPERTY(
        QImage currentFrame
        READ currentFrame
        WRITE setCurrentFrame
        NOTIFY currentFrameChanged)

    Q_PROPERTY(
        quint64 frameRevision
        READ frameRevision
        WRITE setFrameRevision
        NOTIFY frameRevisionChanged)

    Q_PROPERTY(bool hasVideo
        READ hasVideo
        NOTIFY hasVideoChanged)

    Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)
    Q_PROPERTY(bool recordingActionEnabled READ recordingActionEnabled NOTIFY recordingActionEnabledChanged)

    Q_PROPERTY(QString codec READ codec NOTIFY statisticsChanged)
    Q_PROPERTY(QString resolution READ resolution NOTIFY statisticsChanged)
    Q_PROPERTY(double fps READ fps NOTIFY statisticsChanged)
    Q_PROPERTY(double bitrateMbps READ bitrateMbps NOTIFY statisticsChanged)
    Q_PROPERTY(quint64 framesDecoded READ framesDecoded NOTIFY statisticsChanged)
    Q_PROPERTY(quint64 packetsReceived READ packetsReceived NOTIFY statisticsChanged)

public:
    CameraStreamViewModel(const QString& cameraId, StreamingSession* session, QObject* parent = nullptr);

    int state() const;
    QImage currentFrame() const;
    void setCurrentFrame(QImage image);

    quint64 frameRevision() const;
    void setFrameRevision(quint64 frameRevision);

    bool hasVideo() const;

    QString codec() const;
    QString resolution() const;

    double fps() const;
    double bitrateMbps() const;

    quint64 framesDecoded() const;
    quint64 packetsReceived() const;

    bool recording() const;
    bool recordingActionEnabled() const;

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();

signals:
    void stateChanged();
    void currentFrameChanged();
    void frameRevisionChanged();
    void hasVideoChanged();
    void statisticsChanged();
    void recordingActionEnabledChanged();
    void recordingChanged();

private slots:
    void onFrameUpdated();
    void onStateChanged(ConnectionState state);
    void onStatisticsUpdated();
    void onRecordingStateChanged(RecordingState state);

private:
    // non-owning
    StreamingSession* m_streamingSession;
    ConnectionState m_state{ConnectionState::Disconnected};
    QImage m_currentFrame;
    quint64 m_frameRevision = 0;
    QString m_cameraId;
    bool m_hasVideo{};

    RecordingState m_recordingState = RecordingState::Stopped;

};
}
