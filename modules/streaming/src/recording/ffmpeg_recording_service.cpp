#include <vap/streaming/recording/ffmpeg_recording_service.hpp>

#include <vap/streaming/recording/recording_configuration.hpp>

namespace vap
{

FFmpegRecordingService::~FFmpegRecordingService() noexcept = default;

RecordingResult FFmpegRecordingService::startRecording(
    const RecordingConfiguration& configuration)
{
    if (configuration.outputPath.isEmpty())
    {
        return RecordingResult::InvalidConfiguration;
    }

    if (isRecording())
    {
        return RecordingResult::AlreadyRecording;
    }

    m_state = RecordingState::Starting;

    // FFmpeg initialization will happen here in the next commit.

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

}
