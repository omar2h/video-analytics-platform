#pragma once

#include <QObject>

#include <vap/camera/camera_config.hpp>
#include <vap/stream_state.hpp>

namespace vap
{

class ICameraService : public QObject
{
    Q_OBJECT

public:
    explicit ICameraService(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    ~ICameraService() override = default;

    virtual void connect(const CameraConfig& config) = 0;
    virtual void disconnect() = 0;

signals:
    void stateChanged(StreamState state);
};

}
