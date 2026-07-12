#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(ffmpegStreamingLog, "vap.streaming.ffmpeg")

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/error.h>
}

namespace
{

QString ffmpegErrorString(int errorCode)
{
    char buffer[AV_ERROR_MAX_STRING_SIZE];
    av_strerror(errorCode, buffer, sizeof(buffer));
    return QString::fromUtf8(buffer);
}

} // namespace

namespace vap
{

FFmpegStreamingService::FFmpegStreamingService(QObject* parent)
    : IStreamingService(parent)
{
    const int result = avformat_network_init();

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to initialize FFmpeg networking:"
            << ffmpegErrorString(result);
    }
}

FFmpegStreamingService::~FFmpegStreamingService()
{
    disconnectFromStream();

    avformat_network_deinit();
}

void FFmpegStreamingService::connectToStream(const QString &uri)
{
    qInfo(ffmpegStreamingLog) << "Connecting to stream:" << uri;
    disconnectFromStream();
    emit stateChanged(StreamState::Opening);

    const QByteArray uriUtf8 = uri.toUtf8();
    int result = avformat_open_input(
        &m_formatContext,
        uriUtf8.constData(),
        nullptr,
        nullptr);
    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to open stream:"
            << ffmpegErrorString(result);
        emit stateChanged(StreamState::Error);
        return;
    }


    result = avformat_find_stream_info(m_formatContext, nullptr);

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to read stream information:"
            << ffmpegErrorString(result);

        qWarning(ffmpegStreamingLog) << "Failed to open stream";
        disconnectFromStream();

        emit stateChanged(StreamState::Error);

        return;
    }
    qInfo(ffmpegStreamingLog)
        << "Connected to stream successfully.";

    emit stateChanged(StreamState::Connected);
}

void FFmpegStreamingService::disconnectFromStream()
{
    if (!m_formatContext)
        return;

    qInfo(ffmpegStreamingLog)
        << "Disconnecting stream.";

    avformat_close_input(&m_formatContext);

    emit stateChanged(StreamState::Disconnected);
}
}
