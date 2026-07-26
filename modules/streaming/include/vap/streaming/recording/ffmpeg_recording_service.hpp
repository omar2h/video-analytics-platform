#pragma once

#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>


struct AVFormatContext;
struct AVStream;
struct AVPacket;

namespace vap
{
class RecordingConfiguration;
class FFmpegRecordingService
{
public:
    FFmpegRecordingService() noexcept = default;
    ~FFmpegRecordingService() noexcept;

    FFmpegRecordingService(const FFmpegRecordingService&) = delete;
    FFmpegRecordingService& operator=(const FFmpegRecordingService&) = delete;

    FFmpegRecordingService(FFmpegRecordingService&&) = delete;
    FFmpegRecordingService& operator=(FFmpegRecordingService&&) = delete;

    RecordingResult startRecording(
        const RecordingConfiguration& configuration,
        const AVFormatContext& inputContext,
        int videoStreamIndex);

    void stopRecording();

    [[nodiscard]]
    bool isRecording() const noexcept;

    [[nodiscard]]
    RecordingState state() const noexcept;

private:
    RecordingResult initializeOutput(
        const RecordingConfiguration& configuration,
        const AVStream& inputStream);
    void cleanupOutputContext() noexcept;
    void cleanup() noexcept;

private:
    RecordingState m_state {RecordingState::Stopped};
    AVFormatContext* m_outputContext = nullptr;
    AVStream* m_outputStream = nullptr;
};

}
