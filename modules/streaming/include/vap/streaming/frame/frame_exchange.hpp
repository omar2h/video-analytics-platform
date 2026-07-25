#pragma once

#include <QMutex>
#include <QImage>
#include <chrono>

namespace vap
{

struct FrameSnapshot
{
    QImage image;

    uint64_t revision{0};

    std::chrono::steady_clock::time_point decodeTime;

    bool valid{false};
};

class FrameExchange
{
public:
    void publish(QImage frame, std::chrono::steady_clock::time_point decodeTime = std::chrono::steady_clock::now());

    [[nodiscard]]
    FrameSnapshot snapshot() const;

private:
    mutable QMutex m_mutex;

    FrameSnapshot m_snapshot;
};

}
