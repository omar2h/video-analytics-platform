#pragma once

#include <QObject>
#include <QImage>

#include <vap/common/connection_state.hpp>

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
        NOTIFY currentFrameChanged)

    Q_PROPERTY(
        int frameRevision
        READ frameRevision
        NOTIFY frameRevisionChanged)

    Q_PROPERTY(bool hasVideo
        READ hasVideo
        NOTIFY hasVideoChanged)

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
    int frameRevision() const;
    bool hasVideo() const;

    QString codec() const;
    QString resolution() const;

    double fps() const;
    double bitrateMbps() const;

    quint64 framesDecoded() const;
    quint64 packetsReceived() const;

signals:
    void stateChanged();
    void currentFrameChanged();
    void frameRevisionChanged();
    void hasVideoChanged();
    void statisticsChanged();

private slots:
    void onFrameReady(const QImage&);
    void onStateChanged(ConnectionState state);
    void onStatisticsUpdated();

private:
    // non-owning
    StreamingSession* m_streamingSession;
    ConnectionState m_state{ConnectionState::Disconnected};
    QImage m_currentFrame;
    int m_frameRevision = 0;
    QString m_cameraId;
    bool m_hasVideo{};

};
}
