#pragma once

#include <QObject>
#include <atomic>
#include <QMutex>
#include <QWaitCondition>

#include <vap/streaming/streaming_exit_reason.hpp>
#include <vap/streaming/reconnect/reconnect_policy.hpp>
#include <vap/common/connection_state.hpp>
#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>

namespace vap
{
class IStreamingService;
class FrameExchange;
class RecordingConfiguration;
class StreamingWorker : public QObject
{
    Q_OBJECT

public:
    explicit StreamingWorker(
        IStreamingService* streamingService,
        FrameExchange& frameExchange,
        QObject* parent = nullptr);

    void requestCancellation();

public slots:
    void start(const QString& uri);
    void startRecording(
        const RecordingConfiguration& configuration);

    void stopRecording();

signals:
    void frameUpdated();
    void stateChanged(const ConnectionState& state);
    void errorOccurred(const QString& error);
    void recordingStarted(RecordingResult result);
    void recordingStateChanged(RecordingState state);

private:
    bool handleExitReason(StreamingExitReason reason);
    bool waitForRetryDelay();

private:
    IStreamingService* m_streamingService;
    FrameExchange& m_frameExchange;
    ReconnectPolicy m_reconnectPolicy;

    std::atomic_bool m_cancelRequested{false};

    QMutex m_waitMutex;
    QWaitCondition m_waitCondition;
};

}
