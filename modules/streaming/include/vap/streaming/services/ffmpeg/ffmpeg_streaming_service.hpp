#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>

#include <atomic>
#include <QElapsedTimer>

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

    RecordingResult startRecording(
        const RecordingConfiguration& configuration) override;

    void stopRecording() override;

    [[nodiscard]]
    RecordingState recordingState() const noexcept override;

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

private:
    static int interruptCallback(void* opaque);

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
};
}
