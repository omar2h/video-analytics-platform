#pragma once

#include <QObject>
#include <memory>

#include <vap/camera/repositories/i_camera_repository.hpp>

#include "../models/camera_list_model.hpp"

namespace vap
{

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

public:
    explicit CameraManagementViewModel(ICameraRepository* repository, QObject* parent = nullptr);

    CameraListModel* cameraModel() const;

    int selectedIndex() const;
    void setSelectedIndex(int index);

    QString selectedCameraName() const;
    QString selectedCameraUrl() const;

signals:
    void selectedIndexChanged();
    void selectedCameraChanged();

private:
    const Camera* selectedCamera() const;

    ICameraRepository* m_repository{};

    std::unique_ptr<CameraListModel> m_cameraModel;

    QList<Camera> m_cameras;

    int m_selectedIndex{-1};
};

}
