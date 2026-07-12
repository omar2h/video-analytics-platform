#pragma once

#include <memory>

#include "../models/camera_list_model.hpp"

class QQmlApplicationEngine;

namespace vap
{
class MockStreamingService;
class CameraViewModel;
class SQLiteCameraRepository;
class CameraApplicationService;
class CameraManagementViewModel;
class CameraValidator;
class Database;

class ApplicationBootstrap
{
public:
    explicit ApplicationBootstrap(QQmlApplicationEngine& engine);
    ~ApplicationBootstrap();
    void initialize();

private:
    QQmlApplicationEngine& m_engine;

    std::unique_ptr<Database> m_database;
    std::unique_ptr<MockStreamingService> m_streamingService;
    std::unique_ptr<CameraViewModel> m_cameraViewModel;
    std::unique_ptr<SQLiteCameraRepository> m_cameraRepository;
    std::unique_ptr<CameraValidator> m_cameraValidator;
    std::unique_ptr<CameraApplicationService> m_cameraApplicationService;
    std::unique_ptr<CameraManagementViewModel> m_cameraManagementViewModel;
};
}
