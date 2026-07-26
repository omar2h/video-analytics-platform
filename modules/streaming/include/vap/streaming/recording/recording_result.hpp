#pragma once

namespace vap
{

enum class RecordingResult
{
    Success,
    AlreadyRecording,
    InvalidConfiguration,
    NotStreaming,

    OutputContextAllocationFailed,
    OutputStreamCreationFailed,
    CodecParametersCopyFailed,
    FileOpenFailed,
    HeaderWriteFailed
};
}
