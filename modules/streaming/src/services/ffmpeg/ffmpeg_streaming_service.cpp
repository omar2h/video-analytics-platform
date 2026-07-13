#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(ffmpegStreamingLog, "vap.streaming.ffmpeg")

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
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

        cleanupInput();

        return false;
    }

    return true;
}

bool FFmpegStreamingService::findVideoStream()
{
    m_videoStreamIndex = -1;

    for (unsigned int i = 0; i < m_formatContext->nb_streams; ++i)
    {
        const AVStream* stream = m_formatContext->streams[i];

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_videoStreamIndex = static_cast<int>(i);

            qInfo(ffmpegStreamingLog)
                << "Found video stream:" << m_videoStreamIndex;

            return true;
        }
    }

    qWarning(ffmpegStreamingLog)
        << "No video stream found.";

    return false;
}

bool FFmpegStreamingService::createCodecContext()
{
    AVStream* stream = m_formatContext->streams[m_videoStreamIndex];

    AVCodecParameters* codecParameters = stream->codecpar;

    const AVCodec* decoder =
        avcodec_find_decoder(codecParameters->codec_id);

    if (!decoder)
    {
        qWarning(ffmpegStreamingLog)
            << "No decoder found.";

        return false;
    }

    m_codecContext = avcodec_alloc_context3(decoder);

    if (!m_codecContext)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to allocate codec context.";

        return false;
    }

    const int result =
        avcodec_parameters_to_context(
            m_codecContext,
            codecParameters);

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to copy codec parameters:"
            << ffmpegErrorString(result);

        cleanupDecoder();

        return false;
    }

    return true;
}

bool FFmpegStreamingService::openDecoder()
{
    int result =
        avcodec_open2(
            m_codecContext,
            m_codecContext->codec,
            nullptr);

    if (result < 0)
    {
        qWarning(ffmpegStreamingLog)
            << "Failed to open decoder:"
            << ffmpegErrorString(result);

        cleanupDecoder();

        return false;
    }

    qInfo(ffmpegStreamingLog)
        << "Decoder opened successfully.";

    qInfo(ffmpegStreamingLog)
        << "Codec:"
        << m_codecContext->codec->name;

    qInfo(ffmpegStreamingLog)
        << "Resolution:"
        << m_codecContext->width
        << "x"
        << m_codecContext->height;

    return true;
}

bool FFmpegStreamingService::initializeDecoder()
{
    if (!findVideoStream())
    {
        cleanupInput();
        return false;
    }

    if (!createCodecContext())
    {
        cleanupInput();
        return false;
    }

    if (!openDecoder())
    {
        cleanupInput();
        return false;
    }

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
    if (m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
    }

    m_videoStreamIndex = -1;
}
}
