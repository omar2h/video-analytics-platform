#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>

#include <atomic>

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
namespace vap
{
class IFrameConverter;
class FFmpegStreamingService : public IStreamingService
{
    Q_OBJECT

public:
    explicit FFmpegStreamingService(std::unique_ptr<IFrameConverter> frameConverter,
                                    QObject* parent = nullptr);
    ~FFmpegStreamingService() override;

    void connectToStream(const QString& uri) override;
    void disconnectFromStream() override;

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

private:
    AVFormatContext* m_formatContext = nullptr;
    AVCodecContext* m_codecContext = nullptr;
    AVPacket* m_packet = nullptr;
    AVFrame* m_frame = nullptr;

    int m_videoStreamIndex = -1;

    std::unique_ptr<IFrameConverter> m_frameConverter;

    std::atomic_bool m_stopRequested{false};
};
}
