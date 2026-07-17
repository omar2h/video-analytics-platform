#pragma once

#include <QObject>
#include <memory>
#include <map>

class QString;

namespace vap
{
class StreamingManager;
class CameraStreamViewModel;

class LiveMonitoringViewModel : public QObject
{
    Q_OBJECT

public:
    explicit LiveMonitoringViewModel(
        StreamingManager* streamingManager,
        QObject* parent = nullptr);

    Q_INVOKABLE CameraStreamViewModel* streamViewModel(const QString& cameraId);
    Q_INVOKABLE bool hasStream(const QString& cameraId) const;

private:
    StreamingManager* m_streamingManager;

    std::map<QString, std::unique_ptr<CameraStreamViewModel>> m_streamViewModels;
};
}
