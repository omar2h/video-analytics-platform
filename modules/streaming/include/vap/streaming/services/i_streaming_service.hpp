#pragma once

#include <QObject>

#include <vap/streaming/streaming_exit_reason.hpp>
#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>

namespace vap
{
class StreamStatistics;
class RecordingConfiguration;
class IStreamingService : public QObject
{
    Q_OBJECT

public:
    explicit IStreamingService(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~IStreamingService() = default;

    virtual StreamingExitReason stream(const QString& uri) = 0;

    virtual void enqueueStartRecording(const RecordingConfiguration& configuration) = 0;

    virtual void enqueueStopRecording() = 0;

    virtual qint64 recordingDurationSeconds() const = 0;

    [[nodiscard]]
    virtual RecordingState recordingState() const noexcept = 0;

    /// Requests cooperative cancellation of the active streaming session.
    ///
    /// Thread-safe.
    /// May be called from any thread.
    ///
    /// The streaming thread will observe the cancellation request
    /// through the FFmpeg interrupt callback.
    virtual void requestCancellation() = 0;

signals:
    void connected();
    void frameReady(const QImage& frame);
    void statisticsUpdated(const StreamStatistics& statistics);
    void recordingStateChanged(RecordingState state);
    void recordingDurationChanged(qint64 seconds);

};

}
