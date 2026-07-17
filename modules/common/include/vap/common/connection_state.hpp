#pragma once

#include <QObject>

namespace vap
{

Q_NAMESPACE

enum class ConnectionState
{
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Error
};

Q_ENUM_NS(ConnectionState)

} // namespace vap
