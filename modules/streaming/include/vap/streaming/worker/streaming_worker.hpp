#pragma once

#include <QObject>

#include <vap/streaming/streaming_exit_reason.hpp>
#include <vap/streaming/reconnect/reconnect_policy.hpp>
#include <vap/common/connection_state.hpp>

namespace vap
{
class IStreamingService;
class FrameExchange;
class StreamingWorker : public QObject
{
    Q_OBJECT

public:
    explicit StreamingWorker(
        IStreamingService* streamingService,
        FrameExchange& frameExchange,
        QObject* parent = nullptr);

    void requestCancellation();

public slots:
    void start(const QString& uri);

signals:
    void frameUpdated();
    void stateChanged(const ConnectionState& state);
    void errorOccurred(const QString& error);

private:
    bool handleExitReason(StreamingExitReason reason);

    IStreamingService* m_streamingService;
    FrameExchange& m_frameExchange;
    ReconnectPolicy m_reconnectPolicy;
};

}
