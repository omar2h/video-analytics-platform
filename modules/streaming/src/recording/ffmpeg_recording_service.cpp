#include <vap/streaming/recording/ffmpeg_recording_service.hpp>

#include <vap/streaming/recording/recording_configuration.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

namespace vap
{

FFmpegRecordingService::~FFmpegRecordingService() noexcept
{
    cleanup();
}

RecordingResult FFmpegRecordingService::startRecording(
    const RecordingConfiguration& configuration,
    const AVFormatContext& inputContext,
    int videoStreamIndex)
{
    if (isRecording())
    {
        return RecordingResult::AlreadyRecording;
    }

    if (configuration.outputPath.isEmpty())
    {
        return RecordingResult::InvalidConfiguration;
    }

    const AVStream* inputStream = inputContext.streams[videoStreamIndex];

    Q_ASSERT(inputStream);

    RecordingResult result = initializeOutput(configuration, *inputStream);

    if (result != RecordingResult::Success)
    {
        return result;
    }

    m_state = RecordingState::Recording;

    return RecordingResult::Success;
}

void FFmpegRecordingService::stopRecording()
{
    if (m_state != RecordingState::Recording)
    {
        return;
    }

    m_state = RecordingState::Stopping;

    cleanup();

    m_state = RecordingState::Stopped;
}

bool FFmpegRecordingService::isRecording() const noexcept
{
    return m_state == RecordingState::Recording;
}

RecordingState FFmpegRecordingService::state() const noexcept
{
    return m_state;
}

RecordingResult FFmpegRecordingService::initializeOutput(const RecordingConfiguration &configuration, const AVStream &inputStream)
{
    const QByteArray outputPath = configuration.outputPath.toUtf8();

    int result = avformat_alloc_output_context2(
                    &m_outputContext,
                    nullptr,
                    nullptr,
                    outputPath.constData());

    if (result < 0 || !m_outputContext)
    {
        return RecordingResult::OutputContextAllocationFailed;
    }

    m_outputStream = avformat_new_stream(
                        m_outputContext,
                        nullptr);

    if (!m_outputStream)
    {
        cleanup();
        return RecordingResult::OutputStreamCreationFailed;
    }

    result = avcodec_parameters_copy(
                m_outputStream->codecpar,
                inputStream.codecpar);

    if (result < 0)
    {
        cleanup();
        return RecordingResult::CodecParametersCopyFailed;
    }

    m_outputStream->codecpar->codec_tag = 0;
    m_outputStream->time_base = inputStream.time_base;

    result = avio_open(
                &m_outputContext->pb,
                outputPath.constData(),
                AVIO_FLAG_WRITE);

    if (result < 0)
    {
        cleanup();
        return RecordingResult::FileOpenFailed;
    }

    result = avformat_write_header(
                m_outputContext,
                nullptr);

    if (result < 0)
    {
        cleanup();
        return RecordingResult::HeaderWriteFailed;
    }
    return RecordingResult::Success;
}

void FFmpegRecordingService::cleanupOutputContext() noexcept
{
    if (!m_outputContext)
    {
        return;
    }

    if (m_outputContext &&
        m_outputContext->pb &&
        !(m_outputContext->oformat->flags & AVFMT_NOFILE))
    {
        avio_closep(&m_outputContext->pb);
    }

    avformat_free_context(m_outputContext);
    m_outputContext = nullptr;
    m_outputStream = nullptr;
}

void FFmpegRecordingService::cleanup() noexcept
{
    cleanupOutputContext();
}

}
