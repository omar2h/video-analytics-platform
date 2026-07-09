#pragma once

#include <QObject>
#include <memory>

#include <src/models/camera_list_model.hpp>

namespace vap
{
class ICameraApplicationService;
class CameraManagementViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        CameraListModel* cameraModel
        READ cameraModel CONSTANT)

    Q_PROPERTY(
        int selectedIndex
        READ selectedIndex
        WRITE setSelectedIndex
        NOTIFY selectedIndexChanged)

    Q_PROPERTY(
        QString selectedCameraName
        READ selectedCameraName
        NOTIFY selectedCameraChanged)

    Q_PROPERTY(
        QString selectedCameraUrl
        READ selectedCameraUrl
        NOTIFY selectedCameraChanged)

    Q_PROPERTY(
        QString newCameraName
        READ newCameraName
        WRITE setNewCameraName
        NOTIFY newCameraNameChanged)

    Q_PROPERTY(
        QString newCameraUrl
        READ newCameraUrl
        WRITE setNewCameraUrl
        NOTIFY newCameraUrlChanged)

public:
    explicit CameraManagementViewModel(ICameraApplicationService* cameraApplicationService, QObject* parent = nullptr);

    CameraListModel* cameraModel() const;

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString selectedCameraName() const;
    QString selectedCameraUrl() const;

    QString newCameraName() const;
    void setNewCameraName(const QString& name);

    QString newCameraUrl() const;
    void setNewCameraUrl(const QString& url);

    Q_INVOKABLE void addCamera();

signals:
    void selectedIndexChanged();
    void selectedCameraChanged();

    void newCameraNameChanged();
    void newCameraUrlChanged();

private:
    void reloadCameras();

    const Camera* selectedCamera() const;

    ICameraApplicationService* m_cameraApplicationService{};

    std::unique_ptr<CameraListModel> m_cameraModel;

    QList<Camera> m_cameras;

    int m_selectedIndex{-1};

    QString m_newCameraName;
    QString m_newCameraUrl;
};

}
