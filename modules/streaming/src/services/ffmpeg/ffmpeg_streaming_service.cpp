#include <vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp>

#include <vap/streaming/logging.hpp>
Q_LOGGING_CATEGORY(ffmpegStreamingLog, "vap.streaming.ffmpeg")

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/pixdesc.h>
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
        qCWarning(ffmpegStreamingLog)
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
    disconnectFromStream();
    qCInfo(ffmpegStreamingLog)
        << "Connecting to stream:" << uri;

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
        disconnectFromStream();
        emit stateChanged(StreamState::Error);
        return;
    }

    if (!initializePacket())
    {
        disconnectFromStream();
        emit stateChanged(StreamState::Error);
        return;
    }

    if (!initializeFrame())
    {
        disconnectFromStream();
        emit stateChanged(StreamState::Error);
        return;
    }

    qCInfo(ffmpegStreamingLog)
        << "Connected to stream successfully.";

    emit stateChanged(StreamState::Connected);

    while (readNextPacket())
    {
        // Continue reading and decoding packets.
    }

    qCInfo(ffmpegStreamingLog)
        << "Stopped reading packets.";
}

void FFmpegStreamingService::disconnectFromStream()
{
    const bool wasConnected =
        m_formatContext ||
        m_codecContext ||
        m_packet ||
        m_frame;

    if (wasConnected)
    {
        qCInfo(ffmpegStreamingLog)
            << "Disconnecting stream.";
    }
    cleanupFrame();
    cleanupPacket();
    cleanupDecoder();
    cleanupInput();

    if (wasConnected)
        emit stateChanged(StreamState::Disconnected);
}

bool FFmpegStreamingService::openInput(const QString &url)
{
    qCInfo(ffmpegStreamingLog)
        << "Opening stream:" << url;

    int result = avformat_open_input(
        &m_formatContext,
        url.toUtf8().constData(),
        nullptr,
        nullptr);

    if (result < 0)
    {
        qCWarning(ffmpegStreamingLog)
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
        qCWarning(ffmpegStreamingLog)
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

            qCInfo(ffmpegStreamingLog)
                << "Found video stream:" << m_videoStreamIndex;

            return true;
        }
    }

    qCWarning(ffmpegStreamingLog)
        << "No video stream found.";

    return false;
}

bool FFmpegStreamingService::createCodecContext()
{
    const AVStream* stream = m_formatContext->streams[m_videoStreamIndex];

    const AVCodecParameters* codecParameters = stream->codecpar;

    const AVCodec* decoder =
        avcodec_find_decoder(codecParameters->codec_id);

    if (!decoder)
    {
        qCWarning(ffmpegStreamingLog)
            << "No decoder found.";

        return false;
    }

    m_codecContext = avcodec_alloc_context3(decoder);

    if (!m_codecContext)
    {
        qCWarning(ffmpegStreamingLog)
            << "Failed to allocate codec context.";

        return false;
    }

    const int result =
        avcodec_parameters_to_context(
            m_codecContext,
            codecParameters);

    if (result < 0)
    {
        qCWarning(ffmpegStreamingLog)
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
        qCWarning(ffmpegStreamingLog)
            << "Failed to open decoder:"
            << ffmpegErrorString(result);

        cleanupDecoder();

        return false;
    }

    qCInfo(ffmpegStreamingLog)
        << "Decoder opened successfully.";

    qCInfo(ffmpegStreamingLog)
        << "Codec:"
        << m_codecContext->codec->name;

    qCInfo(ffmpegStreamingLog)
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

bool FFmpegStreamingService::initializePacket()
{
    m_packet = av_packet_alloc();

    if (!m_packet)
    {
        qCCritical(ffmpegStreamingLog)
            << "Failed to allocate AVPacket.";

        return false;
    }

    qCInfo(ffmpegStreamingLog)
        << "Packet allocated successfully.";

    return true;
}

bool FFmpegStreamingService::initializeFrame()
{
    m_frame = av_frame_alloc();

    if (!m_frame)
    {
        qCCritical(ffmpegStreamingLog)
            << "Failed to allocate AVFrame.";

        return false;
    }

    qCInfo(ffmpegStreamingLog)
        << "Allocated reusable AVFrame.";

    return true;
}

bool FFmpegStreamingService::readNextPacket()
{
    Q_ASSERT(m_formatContext != nullptr);
    Q_ASSERT(m_packet != nullptr);

    const int result = av_read_frame(m_formatContext, m_packet);

    if (result == AVERROR_EOF)
    {
        qCInfo(ffmpegStreamingLog)
            << "End of stream.";

        return false;
    }

    if (result < 0)
    {
        qCCritical(ffmpegStreamingLog)
            << "Failed to read packet:"
            << ffmpegErrorString(result);

        return false;
    }

    if (m_packet->stream_index == m_videoStreamIndex)
    {
        qCDebug(ffmpegStreamingLog)
            << "Packet:"
            << "PTS =" << m_packet->pts
            << "DTS =" << m_packet->dts
            << "Duration =" << m_packet->duration
            << "Size =" << m_packet->size;

        if (sendPacketToDecoder())
        {
            receiveFrames();
        }
    }

    av_packet_unref(m_packet);

    return true;
}

bool FFmpegStreamingService::sendPacketToDecoder()
{
    Q_ASSERT(m_codecContext != nullptr);
    Q_ASSERT(m_packet != nullptr);
    const int result =
        avcodec_send_packet(m_codecContext, m_packet);

    if (result == 0)
    {
        return true;
    }

    if (result == AVERROR(EAGAIN))
    {
        qCWarning(ffmpegStreamingLog)
            << "Decoder is not ready to accept another packet.";

        return false;
    }

    if (result == AVERROR_EOF)
    {
        qCInfo(ffmpegStreamingLog)
            << "Decoder has been flushed.";

        return false;
    }

    qCCritical(ffmpegStreamingLog)
        << "Failed to send packet to decoder:"
        << ffmpegErrorString(result);

    return false;
}

void FFmpegStreamingService::receiveFrames()
{
    Q_ASSERT(m_codecContext != nullptr);
    Q_ASSERT(m_frame != nullptr);

    while (true)
    {
        const int result =
            avcodec_receive_frame(m_codecContext, m_frame);

        if (result == AVERROR(EAGAIN))
        {
            break;
        }

        if (result == AVERROR_EOF)
        {
            qCInfo(ffmpegStreamingLog)
                << "Decoder reached end of stream.";

            break;
        }

        if (result < 0)
        {
            qCCritical(ffmpegStreamingLog)
                << "Failed to receive decoded frame:"
                << ffmpegErrorString(result);

            break;
        }

        const char* pixelFormat =
            av_get_pix_fmt_name(
                static_cast<AVPixelFormat>(m_frame->format));

        const bool isKeyFrame =
            (m_frame->flags & AV_FRAME_FLAG_KEY) != 0;

        qCDebug(ffmpegStreamingLog)
            << "Decoded frame:"
            << "PTS =" << m_frame->pts
            << "Resolution =" << m_frame->width << "x" << m_frame->height
            << "Pixel Format =" << (pixelFormat ? pixelFormat : "unknown")
            << "Type =" << (isKeyFrame ? "Key" : "Inter");
        av_frame_unref(m_frame);
    }
}

void FFmpegStreamingService::cleanupPacket()
{
    if (!m_packet)
        return;

    av_packet_free(&m_packet);

    qCInfo(ffmpegStreamingLog)
        << "Packet released.";
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

void FFmpegStreamingService::cleanupFrame()
{
    if (!m_frame)
        return;

    av_frame_free(&m_frame);

    qCDebug(ffmpegStreamingLog)
        << "Released AVFrame.";
}
}
