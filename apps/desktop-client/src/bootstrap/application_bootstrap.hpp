#pragma once

#include <memory>

class QQmlApplicationEngine;

namespace vap
{
class MockCameraService;
class CameraViewModel;

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
};
}
