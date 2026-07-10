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
        QString cameraName
        READ cameraName
        WRITE setCameraName
        NOTIFY cameraNameChanged)

    Q_PROPERTY(
        QString cameraUrl
        READ cameraUrl
        WRITE setCameraUrl
        NOTIFY cameraUrlChanged)

public:
    explicit CameraManagementViewModel(ICameraApplicationService* cameraApplicationService, QObject* parent = nullptr);

    CameraListModel* cameraModel() const;

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString selectedCameraName() const;
    QString selectedCameraUrl() const;

    QString cameraName() const;
    void setCameraName(const QString& name);

    QString cameraUrl() const;
    void setCameraUrl(const QString& url);

    Q_INVOKABLE void addCamera();
    Q_INVOKABLE void deleteSelectedCamera();
    Q_INVOKABLE void updateSelectedCamera();
    Q_INVOKABLE void clearSelection();

signals:
    void selectedIndexChanged();
    void selectedCameraChanged();

    void cameraNameChanged();
    void cameraUrlChanged();

private:
    void reloadCameras();
    void loadSelectedCameraIntoForm();
    void clearForm();

    const Camera* selectedCamera() const;

    ICameraApplicationService* m_cameraApplicationService{};

    std::unique_ptr<CameraListModel> m_cameraModel;

    QList<Camera> m_cameras;

    int m_selectedIndex{-1};

    QString m_cameraName;
    QString m_cameraUrl;

};

}
