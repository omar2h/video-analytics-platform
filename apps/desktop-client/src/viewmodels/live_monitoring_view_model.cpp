#include "live_monitoring_view_model.hpp"

#include <src/viewmodels/camera_stream_view_model.hpp>

#include <vap/streaming/manager/streaming_manager.hpp>
#include <vap/streaming/session/streaming_session.hpp>
#include <src/providers/video_frame_provider.hpp>

namespace vap
{

LiveMonitoringViewModel::LiveMonitoringViewModel(
    StreamingManager* streamingManager,
    VideoFrameProvider* provider,
    QObject* parent)
    : QObject(parent)
    , m_streamingManager(streamingManager),
    m_videoFrameProvider(provider)
{
}

CameraStreamViewModel* LiveMonitoringViewModel::streamViewModel(
    const QString& cameraId)
{
    qDebug() << "streamViewModel called for" << cameraId;
    auto it = m_streamViewModels.find(cameraId);

    if (it != m_streamViewModels.end())
        return it->second.get();

    auto* session = m_streamingManager->session(cameraId);

    if (!session) {
        qDebug() << "No session for" << cameraId;
        return nullptr;
    }

    auto viewModel = std::make_unique<CameraStreamViewModel>(cameraId, session);

    auto* streamViewModel = viewModel.get();

    connect(streamViewModel,
            &CameraStreamViewModel::currentFrameChanged,
            this,
            [this, cameraId, streamViewModel]
            {
                if (streamViewModel->currentFrame().isNull())
                {
                    m_videoFrameProvider->clearImage(cameraId);
                }
                else
                {
                    m_videoFrameProvider->setImage(
                        cameraId,
                        streamViewModel->currentFrame());
                }
            });

    m_streamViewModels.emplace(
        cameraId,
        std::move(viewModel));

    return streamViewModel;
}

bool LiveMonitoringViewModel::hasStream(const QString &cameraId) const
{
    return m_streamingManager->contains(cameraId);
}

}
