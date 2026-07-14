#pragma once

namespace vap
{
enum class StreamState
{
    Disconnected,
    Opening,
    Connecting,
    Connected,
    Reconnecting,
    Error
};
}
