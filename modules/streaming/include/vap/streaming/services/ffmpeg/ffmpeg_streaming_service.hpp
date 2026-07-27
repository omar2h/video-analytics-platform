#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

#include <atomic>
#include <QElapsedTimer>
#include <optional>

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
namespace vap
{
class IFrameConverter;
class FFmpegRecordingService;
class FFmpegStreamingService : public IStreamingService
{
    Q_OBJECT

public:
    explicit FFmpegStreamingService(std::unique_ptr<IFrameConverter> frameConverter,
                                    QObject* parent = nullptr);
    ~FFmpegStreamingService() override;

    StreamingExitReason stream(const QString& uri) override;

    [[nodiscard]]
    RecordingState recordingState() const noexcept override;

    RecordingResult requestStartRecording(
        const RecordingConfiguration& configuration);

    void requestStopRecording();

    qint64 recordingDurationSeconds() const override;

    void publishRecordingDurationIfNeeded();

    void requestCancellation() override;

private:
    bool openInput(const QString& url);

    bool readStreamInfo();

    bool findVideoStream();

    bool createCodecContext();

    bool openDecoder();

    bool initializeDecoder();

    bool initializePacket();

    bool initializeFrame();

    bool initializeEverything(const QString& uri);

    bool readNextPacket();

    bool sendPacketToDecoder();

    void receiveFrames();

    // void streamLoop();

    void cleanupFrame();

    void cleanupPacket();

    void cleanupInput();

    void cleanupDecoder();

    void cleanup();

    void publishStatisticsIfNeeded();

    void processPendingCommands();

private:
    static int interruptCallback(void* opaque);

private:
    AVFormatContext* m_formatContext = nullptr;
    AVCodecContext* m_codecContext = nullptr;
    AVPacket* m_packet = nullptr;
    AVFrame* m_frame = nullptr;
    StreamStatistics m_statistics;
    QElapsedTimer m_statisticsTimer;

    int m_videoStreamIndex = -1;

    std::unique_ptr<IFrameConverter> m_frameConverter;
    std::unique_ptr<FFmpegRecordingService> m_recordingService;

    std::atomic_bool m_stopRequested{false};

    std::mutex m_commandMutex;

    std::optional<RecordingConfiguration> m_pendingStartRecording;
    std::optional<RecordingConfiguration> m_recordingRequest;
    bool m_pendingStopRecording = false;

    qint64 m_lastPublishedDuration{};
};
}
