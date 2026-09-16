#pragma once

#include <QObject>
#include <memory>
#include <stop_token>
#include <cstdint>

#include <vap/common/connection_state.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>
#include <vap/streaming/frame/frame_exchange.hpp>
#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>
#include <vap/camera/camera.hpp>

class QThread;
class QImage;
namespace vap
{
class StreamingWorker;
class CameraConfig;
class IStreamingService;

class StreamingSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ConnectionState state
               READ state
               NOTIFY stateChanged)

public:
    StreamingSession(QObject* parent = nullptr);
    ~StreamingSession();

    void start(const Camera& camera);
    void stop();

    void startRecording(const RecordingConfiguration& configuration);

    void stopRecording();

    [[nodiscard]]
    RecordingState recordingState() const noexcept;

    ConnectionState state() const;
    const StreamStatistics& statistics() const;

    QString cameraName() const;
    FrameSnapshot currentFrame() const;

    [[nodiscard]]
    std::uint64_t streamRunId() const;

signals:
    void frameUpdated();
    void stateChanged(const ConnectionState&);
    void errorOccurred(const QString& error);
    void statisticsUpdated(const StreamStatistics& statistics);

    void recordingStateChanged(RecordingState state);
    void recordingDurationChanged(qint64 seconds);

private slots:
    void onStateChanged(const ConnectionState&);
    void onStatisticsUpdated(const StreamStatistics& statistics);
    void onRecordingStateChanged(RecordingState state);

private:
    std::stop_source m_stopSource;

    std::unique_ptr<QThread> m_streamingThread;
    // Borrowed pointers. The worker owns the service as a QObject child.
    // The worker is deleted when the streaming thread finishes.
    IStreamingService* m_streamingService{nullptr};
    StreamingWorker* m_streamingWorker{nullptr};
    Camera m_camera;

    FrameExchange m_frameExchange;
    ConnectionState m_state{ConnectionState::Disconnected};
    RecordingState m_recordingState{RecordingState::Stopped};

    StreamStatistics m_statistics;

    std::uint64_t m_streamRunId{};
};

}
