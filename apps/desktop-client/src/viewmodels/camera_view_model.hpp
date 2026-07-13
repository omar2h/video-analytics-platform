#pragma once

#include <QObject>

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

public:
    explicit CameraViewModel(IStreamingService* streamingService, QObject* parent = nullptr);

    QString stateText() const;

    int state() const;

    Q_INVOKABLE void connectCamera(const QString& url);
    Q_INVOKABLE void disconnectCamera();

signals:
    void stateTextChanged();
    void stateChanged();

private:
    // non-owning
    IStreamingService* m_streamingService;
    QString m_url;
    StreamState m_state{StreamState::Disconnected};
};

}
