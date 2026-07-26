#pragma once

#include <vap/streaming/recording/i_recording_service.hpp>

namespace vap
{

class FFmpegRecordingService : public IRecordingService
{
public:
    FFmpegRecordingService() noexcept = default;
    ~FFmpegRecordingService() noexcept override;

    FFmpegRecordingService(const FFmpegRecordingService&) = delete;
    FFmpegRecordingService& operator=(const FFmpegRecordingService&) = delete;

    FFmpegRecordingService(FFmpegRecordingService&&) = delete;
    FFmpegRecordingService& operator=(FFmpegRecordingService&&) = delete;

    RecordingResult startRecording(
        const RecordingConfiguration& configuration) override;

    void stopRecording() override;

    [[nodiscard]]
    bool isRecording() const noexcept override;

    [[nodiscard]]
    RecordingState state() const noexcept override;

private:
    RecordingState m_state {RecordingState::Stopped};
};

}
