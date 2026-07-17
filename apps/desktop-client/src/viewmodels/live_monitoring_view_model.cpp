#include "live_monitoring_view_model.hpp"

#include <src/viewmodels/camera_stream_view_model.hpp>

#include <vap/streaming/manager/streaming_manager.hpp>
#include <vap/streaming/session/streaming_session.hpp>

namespace vap
{

LiveMonitoringViewModel::LiveMonitoringViewModel(
    StreamingManager* streamingManager,
    QObject* parent)
    : QObject(parent)
    , m_streamingManager(streamingManager)
{
}

CameraStreamViewModel* LiveMonitoringViewModel::streamViewModel(
    const QString& cameraId)
{
    auto it = m_streamViewModels.find(cameraId);

    if (it != m_streamViewModels.end())
        return it->second.get();

    auto* session = m_streamingManager->session(cameraId);

    if (!session)
        return nullptr;

    auto viewModel = std::make_unique<CameraStreamViewModel>(cameraId, session);

    auto* result = viewModel.get();

    m_streamViewModels.emplace(
        cameraId,
        std::move(viewModel));

    return result;
}

bool LiveMonitoringViewModel::hasStream(const QString &cameraId) const
{
    return m_streamingManager->contains(cameraId);
}

}
