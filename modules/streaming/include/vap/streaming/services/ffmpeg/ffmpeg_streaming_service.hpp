#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>

struct AVFormatContext;
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
    AVFormatContext* m_formatContext = nullptr;
};
}
