#pragma once

#include <QObject>
#include <QImage>
#include <memory>

#include <vap/camera/camera.hpp>
#include <vap/common/connection_state.hpp>

namespace vap
{
class StreamingSession;
class CameraItemViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id
        READ id
        CONSTANT)

    Q_PROPERTY(QString name
        READ name
        CONSTANT)

    Q_PROPERTY(QString url
        READ url
        CONSTANT)

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
    CameraItemViewModel(Camera camera, StreamingSession* session, QObject* parent = nullptr);

    int state() const;
    QImage currentFrame() const;
    int frameRevision() const;
    QString id() const;
    QString name() const;
    QString url() const;

    Q_INVOKABLE void connectCamera();
    Q_INVOKABLE void disconnectCamera();

signals:
    void stateChanged();
    void currentFrameChanged();
    void frameRevisionChanged();

private slots:
    void onFrameReady(const QImage&);

private:
    // non-owning
    StreamingSession* m_streamingSession;
    Camera m_camera;
    ConnectionState m_state{ConnectionState::Disconnected};
    QImage m_currentFrame;
    int m_frameRevision = 0;

};
}
