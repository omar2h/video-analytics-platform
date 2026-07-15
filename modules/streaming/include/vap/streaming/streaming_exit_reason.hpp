#pragma once

namespace vap
{

enum class StreamingExitReason
{
    Cancelled,
    NetworkFailure,
    AuthenticationFailure,
    InvalidUrl,
    Timeout,
    StreamEnded,
    UnknownError
};

}
