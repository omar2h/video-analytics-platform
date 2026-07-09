#include "camera_management_view_model.hpp"

#include <vap/camera/services/i_camera_application_service.hpp>

namespace vap
{

CameraManagementViewModel::CameraManagementViewModel(ICameraApplicationService* cameraApplicationService, QObject* parent)
    : QObject(parent),
    m_cameraApplicationService(cameraApplicationService),
    m_cameraModel(std::make_unique<CameraListModel>())
{
    m_cameras = m_cameraApplicationService->cameras();
    m_cameraModel->setCameras(m_cameras);
}

CameraListModel* CameraManagementViewModel::cameraModel() const
{
    return m_cameraModel.get();
}

int CameraManagementViewModel::selectedIndex() const
{
    return m_selectedIndex;
}

void CameraManagementViewModel::setSelectedIndex(int index)
{
    if(m_selectedIndex == index)
        return;
    m_selectedIndex = index;

    emit selectedIndexChanged();
    emit selectedCameraChanged();
    qDebug() << selectedCameraName();
}

QString CameraManagementViewModel::selectedCameraName() const
{
    const auto* camera = selectedCamera();

    if (!camera)
    {
        return {};
    }

    return camera->name;
}

QString CameraManagementViewModel::selectedCameraUrl() const
{
    const auto* camera = selectedCamera();

    if (!camera)
    {
        return {};
    }

    return camera->config.url;
}

const Camera* CameraManagementViewModel::selectedCamera() const
{
    if (m_selectedIndex < 0 ||
        m_selectedIndex >= m_cameras.size())
    {
        return nullptr;
    }

    return &m_cameras[m_selectedIndex];
}

}
