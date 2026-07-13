#include "camera_view_model.hpp"

namespace vap
{

CameraViewModel::CameraViewModel(IStreamingService* streamingService, QObject* parent)
    : QObject(parent), m_streamingService(streamingService)
{
    connect(m_streamingService, &IStreamingService::stateChanged, this,
            [this](StreamState state){
                m_state = state;

                emit stateChanged();
                emit stateTextChanged();
            });
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

void CameraViewModel::connectCamera(const QString& url)
{
    m_streamingService->connectToStream(url);
}

void CameraViewModel::disconnectCamera()
{
    m_streamingService->disconnectFromStream();
}

}

