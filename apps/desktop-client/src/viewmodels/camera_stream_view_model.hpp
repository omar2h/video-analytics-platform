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

public:
    CameraStreamViewModel(const QString& cameraId, StreamingSession* session, QObject* parent = nullptr);

    int state() const;
    QImage currentFrame() const;
    int frameRevision() const;

signals:
    void stateChanged();
    void currentFrameChanged();
    void frameRevisionChanged();

private slots:
    void onFrameReady(const QImage&);

private:
    // non-owning
    StreamingSession* m_streamingSession;
    ConnectionState m_state{ConnectionState::Disconnected};
    QImage m_currentFrame;
    int m_frameRevision = 0;
    QString m_cameraId;

};
}
