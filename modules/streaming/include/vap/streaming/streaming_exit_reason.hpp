#pragma once

namespace vap
{

enum class StreamingExitReason
{
    Cancelled,
    NetworkFailure,
    InitializationFailure,
    AuthenticationFailure,
    InvalidUrl,
    Timeout,
    StreamEnded,
    UnknownError
};

}
