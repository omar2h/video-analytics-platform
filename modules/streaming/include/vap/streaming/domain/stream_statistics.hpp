// stream_statistics.hpp

#pragma once

#include <QMetaType>
#include <QSize>
#include <QString>

namespace vap
{

struct StreamStatistics
{
    QString codec;
    QSize resolution;

    double fps = 0.0;
    double bitrateMbps = 0.0;

    quint64 framesDecoded = 0;
    quint64 packetsReceived = 0;
    quint64 reconnectCount = 0;

    bool recording = false;
};

} // namespace vap

Q_DECLARE_METATYPE(vap::StreamStatistics)
