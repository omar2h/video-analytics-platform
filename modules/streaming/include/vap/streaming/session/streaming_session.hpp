#pragma once

#include <memory>

class QThread;
namespace vap
{
class StreamingWorker;
class CameraConfig;
class IStreamingService;

class StreamingSession
{
public:
    StreamingSession();
    ~StreamingSession();

    void start(const CameraConfig&);
    void stop();
    StreamingWorker* worker() const noexcept;

private:
    std::unique_ptr<QThread> m_streamingThread;
    std::unique_ptr<IStreamingService> m_streamingService;
    std::unique_ptr<StreamingWorker> m_streamingWorker;
};

}
