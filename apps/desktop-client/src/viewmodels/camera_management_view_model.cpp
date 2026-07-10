#include "camera_management_view_model.hpp"

#include <vap/camera/services/i_camera_application_service.hpp>

namespace vap
{

CameraManagementViewModel::CameraManagementViewModel(ICameraApplicationService* cameraApplicationService, QObject* parent)
    : QObject(parent),
    m_cameraApplicationService(cameraApplicationService),
    m_cameraModel(std::make_unique<CameraListModel>())
{
    reloadCameras();
    clearForm();
}

CameraListModel* CameraManagementViewModel::cameraModel() const
{
    return m_cameraModel.get();
}

void CameraManagementViewModel::reloadCameras()
{
    m_cameras = m_cameraApplicationService->cameras();
    m_cameraModel->setCameras(m_cameras);
}

void CameraManagementViewModel::loadSelectedCameraIntoForm()
{
    const auto* camera = selectedCamera();

    if(!camera)
    {
        clearForm();
        return;
    }

    setCameraName(camera->name);
    setCameraUrl(camera->config.url);
}

void CameraManagementViewModel::clearForm()
{
    setCameraName({});
    setCameraUrl({});
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

    loadSelectedCameraIntoForm();

    emit selectedIndexChanged();
    emit selectedCameraChanged();
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

QString CameraManagementViewModel::cameraName() const
{
    return m_cameraName;
}

void CameraManagementViewModel::setCameraName(const QString &name)
{
    if (m_cameraName == name)
        return;
    m_cameraName = name;
    emit cameraNameChanged();
}

QString CameraManagementViewModel::cameraUrl() const
{
    return m_cameraUrl;
}

void CameraManagementViewModel::setCameraUrl(const QString &url)
{
    if (m_cameraUrl == url)
        return;
    m_cameraUrl = url;
    emit cameraUrlChanged();
}

void CameraManagementViewModel::addCamera()
{
    CameraConfig config;
    config.url = m_cameraUrl;

    m_cameraApplicationService->addCamera(
        m_cameraName,
        config);

    reloadCameras();

    clearForm();
}

void CameraManagementViewModel::deleteSelectedCamera()
{
    const auto* camera = selectedCamera();
    if(!camera)
        return;

    m_cameraApplicationService->removeCamera(camera->id);

    reloadCameras();
    clearSelection();
}

void CameraManagementViewModel::updateSelectedCamera()
{
    const auto* camera = selectedCamera();
    if(!camera)
        return;

    CameraConfig config;
    config.url = m_cameraUrl;

    m_cameraApplicationService->updateCamera(camera->id, m_cameraName, config);

    reloadCameras();

    loadSelectedCameraIntoForm();

    emit selectedCameraChanged();
}

void CameraManagementViewModel::clearSelection()
{
    setSelectedIndex(-1);
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
