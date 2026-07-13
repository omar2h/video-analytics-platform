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

void FFmpegStreamingService::connectToStream(const QString& uri)
{
    qInfo(ffmpegStreamingLog)
        << "Connecting to stream:" << uri;

    disconnectFromStream();

    emit stateChanged(StreamState::Opening);

    if (!openInput(uri))
    {
        emit stateChanged(StreamState::Error);
        return;
    }

    if (!readStreamInfo())
    {
        emit stateChanged(StreamState::Error);
        return;
    }

    if (!initializeDecoder())
    {
        emit stateChanged(StreamState::Error);
        return;
    }

    qInfo(ffmpegStreamingLog)
        << "Connected to stream successfully.";

    emit stateChanged(StreamState::Connected);
}

void FFmpegStreamingService::disconnectFromStream()
{

    if (m_formatContext || m_codecContext)
    {
        qInfo(ffmpegStreamingLog)
            << "Disconnecting stream.";
    }

    cleanupDecoder();
    cleanupInput();

    emit stateChanged(StreamState::Disconnected);
}

bool FFmpegStreamingService::openInput(const QString &url)
{
    qInfo(ffmpegStreamingLog)
        << "Opening stream:" << url;

    int result = avformat_open_input(
        &m_formatContext,
        url.toUtf8().constData(),
        nullptr,
        nullptr);

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to open stream:"
            << ffmpegErrorString(result);

        cleanupInput();

        emit stateChanged(StreamState::Error);

        return false;
    }

    return true;
}

bool FFmpegStreamingService::readStreamInfo()
{
    int result =
        avformat_find_stream_info(m_formatContext, nullptr);

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to read stream information: "
            << ffmpegErrorString(result);

        disconnectFromStream();

        return false;
    }

    return true;
}

bool FFmpegStreamingService::initializeDecoder()
{
    return true;
}

void FFmpegStreamingService::cleanupInput()
{
    if (m_formatContext)
    {
        avformat_close_input(&m_formatContext);
        m_formatContext = nullptr;
    }
}

void FFmpegStreamingService::cleanupDecoder()
{

}
}
