#pragma once

#include <memory>

#include "../models/camera_list_model.hpp"

class QQmlApplicationEngine;

namespace vap
{
class MockCameraService;
class CameraViewModel;
class InMemoryCameraRepository;
class CameraManagementViewModel;

class ApplicationBootstrap
{
public:
    explicit ApplicationBootstrap(QQmlApplicationEngine& engine);
    ~ApplicationBootstrap();
    void initialize();

private:
    QQmlApplicationEngine& m_engine;

    std::unique_ptr<MockCameraService> m_cameraService;
    std::unique_ptr<CameraViewModel> m_cameraViewModel;
    std::unique_ptr<InMemoryCameraRepository> m_cameraRepository;
    std::unique_ptr<CameraManagementViewModel> m_cameraManagementViewModel;
};
}
