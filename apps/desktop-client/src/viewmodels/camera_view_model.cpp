#include "camera_view_model.hpp"

#include <vap/streaming/worker/streaming_worker.hpp>

namespace vap
{

CameraViewModel::CameraViewModel(StreamingWorker* streamingWorker, QObject* parent)
    : QObject(parent), m_streamingWorker(streamingWorker)
{
    Q_ASSERT(m_streamingWorker != nullptr);
    connect(m_streamingWorker,
            &StreamingWorker::stateChanged,
            this,
            [this](ConnectionState state) {
                m_state = state;
                emit stateChanged();
                emit stateTextChanged();
            });

    connect(m_streamingWorker,
            &StreamingWorker::frameReady,
            this,
            &CameraViewModel::onFrameReady);
}

QString CameraViewModel::stateText() const
{
    switch (m_state)
    {
    case ConnectionState::Disconnected:
        return "Disconnected";

    case ConnectionState::Connecting:
        return "Connecting";

    case ConnectionState::Connected:
        return "Connected";

    case ConnectionState::Reconnecting:
        return "Reconnecting";

    case ConnectionState::Error:
        return "Error";
    }

    return "Unknown";
}

int CameraViewModel::state() const
{
    return static_cast<int>(m_state);
}

QImage CameraViewModel::currentFrame() const
{
    return m_currentFrame;
}

int CameraViewModel::frameRevision() const
{
    return m_frameRevision;
}

void CameraViewModel::connectCamera(const QString& url)
{
    QMetaObject::invokeMethod(
        m_streamingWorker,
        "start",
        Qt::QueuedConnection,
        Q_ARG(QString, url));
}

void CameraViewModel::disconnectCamera()
{
    QMetaObject::invokeMethod(
        m_streamingWorker,
        "stop",
        Qt::QueuedConnection);
}

void CameraViewModel::onFrameReady(const QImage& frame)
{
    m_currentFrame = frame;
    emit currentFrameChanged();

    ++m_frameRevision;
    emit frameRevisionChanged();
}

}

