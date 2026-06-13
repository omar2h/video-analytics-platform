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
        READ cameraModel CONSTANT
    )

public:
    explicit CameraManagementViewModel(vap::ICameraRepository* repository, QObject* parent = nullptr);

    CameraListModel* cameraModel() const;

private:
    vap::ICameraRepository* m_repository;

    std::unique_ptr<CameraListModel> m_cameraModel;
};

}
