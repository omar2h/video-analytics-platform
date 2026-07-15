#pragma once

#include <memory>

class QThread;
namespace vap
{
class StreamingWorker;
class IStreamingService;

class StreamingSession
{
public:
    StreamingSession();
    ~StreamingSession();

    StreamingWorker* worker() const noexcept;

private:
    std::unique_ptr<QThread> m_streamingThread;
    std::unique_ptr<IStreamingService> m_streamingService;
    std::unique_ptr<StreamingWorker> m_streamingWorker;
};

}
