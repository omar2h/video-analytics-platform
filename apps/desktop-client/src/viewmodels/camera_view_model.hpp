#pragma once

#include <QObject>

#include <vap/camera/i_camera_service.hpp>

namespace vap
{

class CameraViewModel final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url
                   READ url
                   WRITE setUrl
                   NOTIFY urlChanged)

    Q_PROPERTY(QString stateText
                   READ stateText
                   NOTIFY stateTextChanged)

public:
    explicit CameraViewModel(
        ICameraService* cameraService,
        QObject* parent = nullptr);

    QString url() const;
    void setUrl(const QString& url);

    QString stateText() const;

    Q_INVOKABLE void connectCamera();
    Q_INVOKABLE void disconnectCamera();

signals:
    void urlChanged();
    void stateTextChanged();

private:
    // non-owning
    ICameraService* m_cameraService;
    QString m_url;
    QString m_stateText{"Disconnected"};
};

}
