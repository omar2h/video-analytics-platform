#pragma once

#include <QElapsedTimer>

#include <vap/streaming/recording/recording_result.hpp>
#include <vap/streaming/recording/recording_state.hpp>
#include <vap/streaming/recording/recording_configuration.hpp>

extern "C"
{
#include <libavcodec/avcodec.h>
}


struct AVFormatContext;
struct AVStream;
struct AVPacket;

namespace vap
{
class FFmpegRecordingService
{
public:
    FFmpegRecordingService() noexcept = default;
    ~FFmpegRecordingService() noexcept;

    FFmpegRecordingService(const FFmpegRecordingService&) = delete;
    FFmpegRecordingService& operator=(const FFmpegRecordingService&) = delete;

    FFmpegRecordingService(FFmpegRecordingService&&) = delete;
    FFmpegRecordingService& operator=(FFmpegRecordingService&&) = delete;

    RecordingResult requestRecording(
        const RecordingConfiguration& configuration,
        const AVFormatContext& inputContext,
        int videoStreamIndex);

    void stopRecording();

    qint64 recordingDurationSeconds() const;

    [[nodiscard]]
    RecordingState state() const noexcept;

    void handleVideoPacket(const AVPacket& packet);

private:
    RecordingResult initializeRecording(
        const RecordingConfiguration& configuration,
        const AVStream& inputStream);
    bool writePacket(const AVPacket &packet);
    void cleanupOutputContext() noexcept;
    void cleanup() noexcept;

private:
    QElapsedTimer m_recordingTimer;

    RecordingState m_state{RecordingState::Stopped};

    std::optional<RecordingConfiguration> m_pendingConfiguration;

    const AVStream* m_inputStream = nullptr;

    AVFormatContext* m_outputContext = nullptr;
    AVStream* m_outputStream = nullptr;
    AVRational m_inputTimeBase{};
};

}
