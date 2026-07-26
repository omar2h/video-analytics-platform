#pragma once

#include <vap/streaming/recording/recording_state.hpp>
#include <vap/streaming/recording/recording_result.hpp>

namespace vap
{

class RecordingConfiguration;
class IRecordingService
{
public:
    IRecordingService() = default;
    virtual ~IRecordingService() noexcept = default;

    IRecordingService(const IRecordingService&) = delete;
    IRecordingService& operator=(const IRecordingService&) = delete;

    IRecordingService(IRecordingService&&) = delete;
    IRecordingService& operator=(IRecordingService&&) = delete;

    virtual RecordingResult startRecording(
        const RecordingConfiguration& configuration) = 0;

    virtual void stopRecording() = 0;

    [[nodiscard]]
    virtual bool isRecording() const noexcept = 0;

    [[nodiscard]]
    virtual RecordingState state() const noexcept = 0;

};

}
