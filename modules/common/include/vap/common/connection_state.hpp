#pragma once

namespace vap
{

enum class ConnectionState
{
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Error
};

}
