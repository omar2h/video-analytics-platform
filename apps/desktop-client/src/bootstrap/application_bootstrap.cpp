#include "application_bootstrap.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <vap/camera/mock_camera_service.hpp>
#include <vap/camera/repositories/in_memory_camera_repository.hpp>

#include "../viewmodels/camera_view_model.hpp"

namespace vap
{
ApplicationBootstrap::ApplicationBootstrap(QQmlApplicationEngine& engine)
    : m_engine(engine) {}

ApplicationBootstrap::~ApplicationBootstrap() = default;

void ApplicationBootstrap::initialize()
{
    m_cameraService = std::make_unique<MockCameraService>();
    m_cameraViewModel = std::make_unique<CameraViewModel>(m_cameraService.get());
    m_cameraListModel = std::make_unique<CameraListModel>();
    m_cameraRepository = std::make_unique<InMemoryCameraRepository>();

    m_cameraListModel->setCameras(m_cameraRepository->cameras());

    m_engine.rootContext()->setContextProperty(
        "cameraViewModel", m_cameraViewModel.get());
    m_engine.rootContext()->setContextProperty(
        "cameraModel", m_cameraListModel.get());
}

}
