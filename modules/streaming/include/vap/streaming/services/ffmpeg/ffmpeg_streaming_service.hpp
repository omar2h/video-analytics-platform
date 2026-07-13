#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>

struct AVFormatContext;
struct AVCodecContext;
namespace vap
{

class FFmpegStreamingService : public IStreamingService
{
    Q_OBJECT

public:
    explicit FFmpegStreamingService(QObject* parent = nullptr);
    ~FFmpegStreamingService() override;

    void connectToStream(const QString& uri) override;
    void disconnectFromStream() override;

private:
    bool openInput(const QString& url);

    bool readStreamInfo();

    bool initializeDecoder();

    void cleanupInput();

    void cleanupDecoder();

private:
    AVFormatContext* m_formatContext = nullptr;

    AVCodecContext* m_codecContext = nullptr;

    int m_videoStreamIndex = -1;
};
}
