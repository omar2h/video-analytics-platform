#pragma once

namespace vap
{

enum class RecordingResult
{
    Success,
    AlreadyRecording,
    InvalidConfiguration,

    OutputContextAllocationFailed,
    OutputStreamCreationFailed,
    CodecParametersCopyFailed,
    FileOpenFailed,
    HeaderWriteFailed
};
}
