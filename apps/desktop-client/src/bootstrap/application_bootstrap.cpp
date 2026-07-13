#include "application_bootstrap.hpp"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <vap/streaming/services/mock_streaming_service.hpp>
#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>
#include <vap/streaming/frame/ffmpeg_frame_converter.hpp>
#include <vap/camera/repositories/sqlite_camera_repository.hpp>
#include <vap/camera/validation/camera_validator.hpp>
#include <vap/camera/services/camera_application_service.hpp>
#include <vap/database/database.hpp>

#include <src/viewmodels/camera_view_model.hpp>
#include <src/viewmodels/camera_management_view_model.hpp>

namespace vap
{
ApplicationBootstrap::ApplicationBootstrap(QQmlApplicationEngine& engine)
    : m_engine(engine) {}

ApplicationBootstrap::~ApplicationBootstrap() = default;

void ApplicationBootstrap::initialize()
{
    m_streamingService = std::make_unique<FFmpegStreamingService>(std::make_unique<FFmpegFrameConverter>());
    m_cameraViewModel = std::make_unique<CameraViewModel>(m_streamingService.get());

    m_database = std::make_unique<Database>("video_analytics.db");

    if (!m_database->open())
    {
        qFatal("Failed to open database.");
    }

    m_cameraRepository = std::make_unique<SQLiteCameraRepository>(*m_database);

    m_cameraValidator = std::make_unique<CameraValidator>();
    m_cameraApplicationService = std::make_unique<CameraApplicationService>(m_cameraRepository.get(), m_cameraValidator.get());
    m_cameraManagementViewModel = std::make_unique<CameraManagementViewModel>(m_cameraApplicationService.get());

    m_engine.rootContext()->setContextProperty("cameraViewModel", m_cameraViewModel.get());
    m_engine.rootContext()->setContextProperty("cameraManagementViewModel", m_cameraManagementViewModel.get());
}

}
