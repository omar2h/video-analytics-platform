#pragma once

#include <QObject>
#include <QImage>

#include <vap/streaming/services/i_streaming_service.hpp>

namespace vap
{

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

public:
    explicit CameraViewModel(IStreamingService* streamingService, QObject* parent = nullptr);

    QString stateText() const;
    int state() const;
    QImage currentFrame() const;

    Q_INVOKABLE void connectCamera(const QString& url);
    Q_INVOKABLE void disconnectCamera();

private slots:
    void onFrameReady(const QImage& frame);

signals:
    void stateTextChanged();
    void stateChanged();
    void currentFrameChanged();

private:
    // non-owning
    IStreamingService* m_streamingService;
    QString m_url;
    StreamState m_state{StreamState::Disconnected};
    QImage m_currentFrame;
};

}
