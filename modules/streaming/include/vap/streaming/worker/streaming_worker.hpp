#pragma once

#include <QObject>
#include <condition_variable>
#include <mutex>
#include <stop_token>
#include <cstdint>

#include <vap/streaming/streaming_exit_reason.hpp>
#include <vap/streaming/reconnect/reconnect_policy.hpp>
#include <vap/common/connection_state.hpp>
#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

namespace vap
{
class IStreamingService;
class FrameExchange;
class StreamingWorker : public QObject
{
    Q_OBJECT

public:
    explicit StreamingWorker(
        IStreamingService* streamingService,
        FrameExchange& frameExchange,
        QObject* parent = nullptr);

    void start(
        const QString& uri,
        std::stop_token stopToken,
        std::uint64_t streamRunId);

signals:
    void frameUpdated();
    void stateChanged(const ConnectionState& state);
    void errorOccurred(const QString& error);
    void recordingStarted(RecordingResult result);
    void recordingStateChanged(RecordingState state);

private:
    bool handleExitReason(StreamingExitReason reason);
    bool waitForRetryDelay(std::stop_token stopToken);

private:
    IStreamingService* m_streamingService;
    FrameExchange& m_frameExchange;
    ReconnectPolicy m_reconnectPolicy;

    std::mutex m_waitMutex;
    std::condition_variable_any m_waitCondition;

    std::uint64_t m_streamRunId{};
};

}
