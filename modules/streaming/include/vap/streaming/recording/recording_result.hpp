#pragma once

namespace vap
{

enum class RecordingResult
{
    Success,
    AlreadyRecording,
    InvalidConfiguration,
    InternalError,
    FileOpenFailed,
    StreamInitializationFailed,
    HeaderWriteFailed
};

}
