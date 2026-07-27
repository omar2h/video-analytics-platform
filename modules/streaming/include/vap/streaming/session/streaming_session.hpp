#pragma once

#include <QObject>
#include <memory>

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
    std::unique_ptr<QThread> m_streamingThread;
    std::unique_ptr<IStreamingService> m_streamingService;
    std::unique_ptr<StreamingWorker> m_streamingWorker;
    Camera m_camera;

    FrameExchange m_frameExchange;
    ConnectionState m_state{ConnectionState::Disconnected};
    RecordingState m_recordingState{RecordingState::Stopped};

    StreamStatistics m_statistics;
};

}
