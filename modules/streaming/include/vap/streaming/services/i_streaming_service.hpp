#pragma once

#include <QObject>

#include <vap/common/connection_state.hpp>

namespace vap
{

class IStreamingService : public QObject
{
    Q_OBJECT

public:
    explicit IStreamingService(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~IStreamingService() = default;

    virtual void connectToStream(const QString& uri) = 0;

    /// Requests cooperative cancellation of the active streaming session.
    ///
    /// Thread-safe.
    /// May be called from any thread.
    ///
    /// The streaming thread will observe the cancellation request
    /// through the FFmpeg interrupt callback.
    virtual void requestCancellation() = 0;

signals:
    void stateChanged(ConnectionState state);
    void frameReady(const QImage& frame);
    void errorOccurred(const QString& error);

};

}
