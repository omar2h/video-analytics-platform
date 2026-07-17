#pragma once

#include <QObject>
#include <memory>
#include <map>

class QString;

namespace vap
{
class StreamingManager;
class CameraStreamViewModel;
class VideoFrameProvider;

class LiveMonitoringViewModel : public QObject
{
    Q_OBJECT

public:
    explicit LiveMonitoringViewModel(
        StreamingManager* streamingManager,
        VideoFrameProvider* provider,
        QObject* parent = nullptr);

    Q_INVOKABLE CameraStreamViewModel* streamViewModel(const QString& cameraId);
    Q_INVOKABLE bool hasStream(const QString& cameraId) const;

private:
    StreamingManager* m_streamingManager;
    VideoFrameProvider* m_videoFrameProvider;

    std::map<QString, std::unique_ptr<CameraStreamViewModel>> m_streamViewModels;
};
}
