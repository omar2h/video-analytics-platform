#include "camera_view_model.hpp"

namespace vap
{

CameraViewModel::CameraViewModel(IStreamingService* cameraService, QObject* parent)
    : QObject(parent), m_cameraService(cameraService)
{
    connect(m_cameraService, &IStreamingService::stateChanged, this,
            [this](StreamState state){
                m_state = state;

                emit stateChanged();
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
    switch (m_state)
    {
    case StreamState::Disconnected:
        return "Disconnected";

    case StreamState::Connecting:
        return "Connecting";

    case StreamState::Connected:
        return "Connected";

    case StreamState::Reconnecting:
        return "Reconnecting";

    case StreamState::Error:
        return "Error";
    }

    return "Unknown";
}

int CameraViewModel::state() const
{
    return static_cast<int>(m_state);
}

void CameraViewModel::connectCamera()
{

}

void CameraViewModel::disconnectCamera()
{
    m_cameraService->disconnect();
}

}

