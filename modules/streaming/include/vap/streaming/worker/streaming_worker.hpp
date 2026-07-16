#pragma once

#include <QObject>

#include <vap/streaming/streaming_exit_reason.hpp>
#include <vap/streaming/reconnect/reconnect_policy.hpp>
#include <vap/common/connection_state.hpp>

namespace vap
{
class IStreamingService;
class StreamingWorker : public QObject
{
    Q_OBJECT

public:
    explicit StreamingWorker(
        IStreamingService* streamingService,
        QObject* parent = nullptr);

    void requestCancellation();

public slots:
    void start(const QString& uri);

signals:
    void frameReady(const QImage& image);
    void stateChanged(const ConnectionState& state);
    void errorOccurred(const QString& error);

private:
    bool handleExitReason(StreamingExitReason reason);

    IStreamingService* m_streamingService;
    ReconnectPolicy m_reconnectPolicy;
};

}
