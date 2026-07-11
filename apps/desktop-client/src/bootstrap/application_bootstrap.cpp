#include "application_bootstrap.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <vap/camera/mock_camera_service.hpp>
#include <vap/camera/repositories/in_memory_camera_repository.hpp>
#include <vap/camera/validation/camera_validator.hpp>
#include <vap/camera/services/camera_application_service.hpp>

#include <src/viewmodels/camera_view_model.hpp>
#include <src/viewmodels/camera_management_view_model.hpp>

namespace vap
{
ApplicationBootstrap::ApplicationBootstrap(QQmlApplicationEngine& engine)
    : m_engine(engine) {}

ApplicationBootstrap::~ApplicationBootstrap() = default;

void ApplicationBootstrap::initialize()
{
    m_cameraService = std::make_unique<MockCameraService>();
    m_cameraViewModel = std::make_unique<CameraViewModel>(m_cameraService.get());
    m_cameraRepository = std::make_unique<InMemoryCameraRepository>();
    m_cameraValidator = std::make_unique<CameraValidator>();
    m_cameraApplicationService = std::make_unique<CameraApplicationService>(m_cameraRepository.get(), m_cameraValidator.get());
    m_cameraManagementViewModel = std::make_unique<CameraManagementViewModel>(m_cameraApplicationService.get());

    m_engine.rootContext()->setContextProperty("cameraViewModel", m_cameraViewModel.get());
    m_engine.rootContext()->setContextProperty("cameraManagementViewModel", m_cameraManagementViewModel.get());


}

}
