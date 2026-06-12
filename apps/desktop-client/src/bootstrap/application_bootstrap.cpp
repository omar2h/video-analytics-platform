#include "application_bootstrap.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <vap/camera/mock_camera_service.hpp>

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

    m_engine.rootContext()->setContextProperty(
        "cameraViewModel", m_cameraViewModel.get());
}

}
