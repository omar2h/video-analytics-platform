#include "camera_view_model.hpp"

namespace vap
{

CameraViewModel::CameraViewModel(ICameraService* cameraService, QObject* parent)
    : QObject(parent), m_cameraService(cameraService)
{
    connect(m_cameraService, &ICameraService::stateChanged, this,
            [this](StreamState state){
                switch(state)
                {
                    case StreamState::Disconnected:
                        m_stateText = "Disconnected";
                        break;
                    case StreamState::Connecting:
                        m_stateText = "Connecting";
                        break;
                    case StreamState::Connected:
                        m_stateText = "Connected";
                        break;
                    case StreamState::Reconnecting:
                        m_stateText = "Reconnecting";
                        break;
                    case StreamState::Error:
                        m_stateText = "Error";
                        break;
                }
                emit stateTextChanged();
            });
}

QString CameraViewModel::url() const
{
    return m_url;
}

void CameraViewModel::setUrl(const QString& url)
{
    if(m_url == url)
        return;
    m_url = url;
    emit urlChanged();
}

QString CameraViewModel::stateText() const
{
    return m_stateText;
}

void CameraViewModel::connectCamera()
{
    CameraConfig config;
    config.url = m_url;

    m_cameraService->connect(config);
}

void CameraViewModel::disconnectCamera()
{
    m_cameraService->disconnect();
}

}

