#include "camera_management_view_model.hpp"

#include <vap/camera/services/i_camera_application_service.hpp>
#include <vap/streaming/manager/streaming_manager.hpp>
#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{

CameraManagementViewModel::CameraManagementViewModel(ICameraApplicationService* cameraApplicationService,
                                                     StreamingManager* streamingManager, QObject* parent)
    : QObject(parent),
    m_cameraApplicationService(cameraApplicationService),
    m_streamingManager(streamingManager),
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

CameraValidationError CameraManagementViewModel::validationError() const
{
    return m_validationError;
}

void CameraManagementViewModel::setValidationError(CameraValidationError error)
{
    if (m_validationError == error)
            return;
    m_validationError = error;
    emit validationErrorChanged();
}

QString CameraManagementViewModel::validationMessage() const
{
    switch(m_validationError)
    {
    case CameraValidationError::EmptyName:
        return tr("Camera name is required.");

    case CameraValidationError::EmptyUrl:
        return tr("Camera URL is required.");

    case CameraValidationError::InvalidUrlScheme:
        return tr("Invalid URL");

    case CameraValidationError::None:
        return {};
    }

    return {};
}

void CameraManagementViewModel::addCamera()
{
    CameraConfig config;
    config.url = m_cameraUrl;

    const auto validation = m_cameraApplicationService->addCamera(
        m_cameraName,
        config);

    setValidationError(validation.error);

    if(validation.error != CameraValidationError::None)
        return;

    reloadCameras();

    clearForm();
}

void CameraManagementViewModel::connectSelectedCamera()
{
    const Camera* camera = selectedCamera();

    if (!camera)
        return;

    m_streamingManager->startStreaming(*camera);
}

void CameraManagementViewModel::disconnectSelectedCamera()
{
    const Camera* camera = selectedCamera();
    m_streamingManager->stopStreaming(camera->id);
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

    const auto validation = m_cameraApplicationService->updateCamera(camera->id, m_cameraName, config);

    setValidationError(validation.error);

    if(validation.error != CameraValidationError::None)
        return;

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
