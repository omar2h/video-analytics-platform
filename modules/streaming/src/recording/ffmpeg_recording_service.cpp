#include <vap/streaming/recording/ffmpeg_recording_service.hpp>

#include <vap/streaming/recording/recording_configuration.hpp>

extern "C"
{
#include <libavformat/avformat.h>
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

    // FFmpeg trailer writing will happen here.

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
    Q_UNUSED(configuration);
    Q_UNUSED(inputStream);

    // avformat_alloc_output_context2();
    // avformat_new_stream();
    // avcodec_parameters_copy();
    // avio_open();
    // avformat_write_header();
    return RecordingResult::Success;
}

void FFmpegRecordingService::cleanupOutputContext() noexcept
{
    if (!m_outputContext)
    {
        return;
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
