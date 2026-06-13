#include "camera_management_view_model.hpp"

namespace vap
{

CameraManagementViewModel::CameraManagementViewModel(vap::ICameraRepository* repository, QObject* parent)
    : QObject(parent),
    m_repository(repository),
    m_cameraModel(std::make_unique<CameraListModel>())
{
    m_cameraModel->setCameras(m_repository->cameras());
}

CameraListModel* CameraManagementViewModel::cameraModel() const
{
    return m_cameraModel.get();
}

}
