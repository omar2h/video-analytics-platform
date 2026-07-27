#include <vap/streaming/recording/ffmpeg_recording_service.hpp>

#include <vap/streaming/logging.hpp>
Q_LOGGING_CATEGORY(ffmpegRecordingLog, "vap.streaming.recording")

#include <vap/streaming/recording/recording_configuration.hpp>

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

FFmpegRecordingService::~FFmpegRecordingService() noexcept
{
    cleanup();
}

RecordingResult FFmpegRecordingService::requestRecording(
    const RecordingConfiguration& configuration,
    const AVFormatContext& inputContext,
    int videoStreamIndex)
{
    if (m_state != RecordingState::Stopped)
    {
        return RecordingResult::AlreadyRecording;
    }

    if (configuration.outputPath.isEmpty())
    {
        return RecordingResult::InvalidConfiguration;
    }

    // Save everything needed to start later.
    m_pendingConfiguration = configuration;
    m_inputStream = inputContext.streams[videoStreamIndex];

    qCInfo(ffmpegRecordingLog)
        << "Recording requested. Waiting for first keyframe.";

    m_state = RecordingState::Starting;

    return RecordingResult::Success;
}

void FFmpegRecordingService::stopRecording()
{
    if (m_state == RecordingState::Starting)
    {
        cleanup();
        return;
    }

    if (m_state != RecordingState::Recording)
    {
        return;
    }

    m_state = RecordingState::Stopping;

    qCInfo(ffmpegRecordingLog)
        << "Stopping recording.";

    if (m_outputContext)
    {
        const int result = av_write_trailer(m_outputContext);

        if (result < 0)
        {
            qCWarning(ffmpegRecordingLog)
                << "Failed to write trailer:"
                << ffmpegErrorString(result);
        }
    }

    cleanup();

    m_state = RecordingState::Stopped;
}

qint64 FFmpegRecordingService::recordingDurationSeconds() const
{
    if (m_state != RecordingState::Recording)
    {
        return 0;
    }

    return m_recordingTimer.elapsed() / 1000;
}

RecordingState FFmpegRecordingService::state() const noexcept
{
    return m_state;
}

void FFmpegRecordingService::handleVideoPacket(const AVPacket& packet)
{
    switch (m_state)
    {
    case RecordingState::Stopped:
        return;

    case RecordingState::Starting:
    {
        const bool isKeyFrame = (packet.flags & AV_PKT_FLAG_KEY) != 0;

        if (!isKeyFrame)
        {
            return;
        }

        Q_ASSERT(m_pendingConfiguration.has_value());
        Q_ASSERT(m_inputStream != nullptr);

        const auto result =
            initializeRecording(*m_pendingConfiguration, *m_inputStream);

        if (result != RecordingResult::Success)
        {
            cleanup();
            m_state = RecordingState::Error;
            return;
        }

        m_recordingTimer.start();

        m_pendingConfiguration.reset();

        m_state = RecordingState::Recording;

        if (!writePacket(packet))
        {
            cleanup();

            m_state = RecordingState::Error;
            return;
        }

        return;
    }

    case RecordingState::Recording:
    {
        if (!writePacket(packet))
        {
            cleanup();

            m_state = RecordingState::Error;
            return;
        }
        return;
    }

    case RecordingState::Stopping:
        return;

    case RecordingState::Error:
        return;
    }
}

bool FFmpegRecordingService::writePacket(const AVPacket &packet)
{
    if (m_state != RecordingState::Recording ||
        !m_outputContext ||
        !m_outputStream)
    {
        return false;
    }

    AVPacket outputPacket {};

    if (av_packet_ref(&outputPacket, &packet) < 0)
    {
        return false;
    }
    av_packet_rescale_ts(
        &outputPacket,
        m_inputTimeBase,
        m_outputStream->time_base);

    outputPacket.stream_index = m_outputStream->index;

    const int result =
        av_interleaved_write_frame(
            m_outputContext,
            &outputPacket);

    const bool success = (result >= 0);

    if (result < 0)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to write packet:"
            << ffmpegErrorString(result);
    }

    av_packet_unref(&outputPacket);

    return success;
}

RecordingResult FFmpegRecordingService::initializeRecording(const RecordingConfiguration &configuration, const AVStream &inputStream)
{
    const QByteArray outputPath = configuration.outputPath.toUtf8();

    int result = avformat_alloc_output_context2(
                    &m_outputContext,
                    nullptr,
                    nullptr,
                    outputPath.constData());

    if (result < 0 || !m_outputContext)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to allocate output context:"
            << ffmpegErrorString(result);

        return RecordingResult::OutputContextAllocationFailed;
    }

    m_outputStream = avformat_new_stream(
                        m_outputContext,
                        nullptr);

    if (!m_outputStream)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to create output stream.";

        cleanup();
        return RecordingResult::OutputStreamCreationFailed;
    }

    result = avcodec_parameters_copy(
                m_outputStream->codecpar,
                inputStream.codecpar);

    if (result < 0)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to copy codec parameters:"
            << ffmpegErrorString(result);

        cleanup();
        return RecordingResult::CodecParametersCopyFailed;
    }

    m_outputStream->codecpar->codec_tag = 0;
    m_outputStream->time_base = inputStream.time_base;
    m_inputTimeBase = inputStream.time_base;

    result = avio_open(
                &m_outputContext->pb,
                outputPath.constData(),
                AVIO_FLAG_WRITE);

    if (result < 0)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to open output file:"
            << ffmpegErrorString(result);

        cleanup();
        return RecordingResult::FileOpenFailed;
    }

    result = avformat_write_header(
                m_outputContext,
                nullptr);

    if (result < 0)
    {
        qCWarning(ffmpegRecordingLog)
            << "Failed to write output header:"
            << ffmpegErrorString(result);

        cleanup();
        return RecordingResult::HeaderWriteFailed;
    }
    qCInfo(ffmpegRecordingLog)
        << "Recording started:"
        << configuration.outputPath;

    return RecordingResult::Success;
}

void FFmpegRecordingService::cleanupOutputContext() noexcept
{
    if (!m_outputContext)
    {
        return;
    }

    if (m_outputContext->pb &&
        !(m_outputContext->oformat->flags & AVFMT_NOFILE))
    {
        avio_closep(&m_outputContext->pb);
    }

    avformat_free_context(m_outputContext);
    m_outputContext = nullptr;
    m_outputStream = nullptr;
    m_inputTimeBase = {};
}

void FFmpegRecordingService::cleanup() noexcept
{
    cleanupOutputContext();
    m_pendingConfiguration.reset();
    m_inputStream = nullptr;
    m_state = RecordingState::Stopped;
    m_recordingTimer.invalidate();
}

}
