#pragma once

#include <QObject>
#include <QImage>

#include <vap/common/connection_state.hpp>

namespace vap
{
class StreamingWorker;
class CameraViewModel final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString stateText
                   READ stateText
                   NOTIFY stateTextChanged)

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
    explicit CameraViewModel(StreamingWorker* streamingWorker, QObject* parent = nullptr);

    QString stateText() const;
    int state() const;
    QImage currentFrame() const;
    int frameRevision() const;

    Q_INVOKABLE void connectCamera(const QString& url);
    Q_INVOKABLE void disconnectCamera();

private slots:
    void onFrameReady(const QImage& frame);

signals:
    void stateTextChanged();
    void stateChanged();
    void currentFrameChanged();
    void frameRevisionChanged();

private:
    // non-owning
    StreamingWorker* m_streamingWorker;
    QString m_url;
    ConnectionState m_state{ConnectionState::Disconnected};
    QImage m_currentFrame;
    int m_frameRevision = 0;
};

}
