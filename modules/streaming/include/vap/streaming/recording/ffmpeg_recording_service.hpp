#pragma once

#include <vap/streaming/recording/i_recording_service.hpp>


struct AVFormatContext;
struct AVStream;

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
    void cleanupOutputContext() noexcept;
    void cleanup() noexcept;

private:
    RecordingState m_state {RecordingState::Stopped};
    AVFormatContext* m_outputContext = nullptr;
    AVStream* m_outputStream = nullptr;
};

}
