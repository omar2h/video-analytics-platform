#include <vap/streaming/frame/ffmpeg_frame_converter.hpp>

#include <vap/streaming/logging.hpp>

extern "C"
{
#include <libavutil/pixdesc.h>
}


namespace vap
{

FFmpegFrameConverter::~FFmpegFrameConverter()
{
    sws_freeContext(m_swsContext);
}

QImage FFmpegFrameConverter::convert(const AVFrame* frame)
{
    if (!frame)
    {
        qCWarning(ffmpegStreamingLog)
            << "Cannot convert a null AVFrame.";

        return {};
    }

    if (!ensureContext(frame))
    {
        return {};
    }

    QImage image(
        frame->width,
        frame->height,
        QImage::Format_RGB888);

    if (image.isNull())
    {
        qCCritical(ffmpegStreamingLog)
            << "Failed to allocate destination image for"
            << frame->width << "x" << frame->height;

        return {};
    }

    uint8_t* destination[] =
        {
            image.bits(),
            nullptr,
            nullptr,
            nullptr
        };

    int destinationLinesize[] =
        {
            image.bytesPerLine(),
            0,
            0,
            0
        };

    const int result = sws_scale(
        m_swsContext,
        frame->data,
        frame->linesize,
        0,
        frame->height,
        destination,
        destinationLinesize);

    if (result < 0)
    {
        const auto* pixelFormatName =
            av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame->format));

        qCCritical(ffmpegStreamingLog)
            << "Failed to convert frame:"
            << frame->width << "x" << frame->height
            << "pixel format:"
            << (pixelFormatName ? pixelFormatName : "unknown")
            << "error:"
            << result;

        return {};
    }

    return image;
}

bool FFmpegFrameConverter::ensureContext(const AVFrame* frame)
{
    m_swsContext = sws_getCachedContext(
        m_swsContext,
        frame->width,
        frame->height,
        static_cast<AVPixelFormat>(frame->format),
        frame->width,
        frame->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        nullptr,
        nullptr,
        nullptr);

    if (!m_swsContext)
    {
        qCCritical(ffmpegStreamingLog)
            << "Failed to create SwsContext for"
            << frame->width << "x" << frame->height
            << "pixel format:"
            << av_get_pix_fmt_name(
                   static_cast<AVPixelFormat>(frame->format));

        return false;
    }

    return true;
}

}
