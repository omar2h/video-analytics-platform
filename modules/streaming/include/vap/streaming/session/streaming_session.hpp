#pragma once

#include <QObject>
#include <memory>

#include <vap/common/connection_state.hpp>
#include <vap/streaming/domain/stream_statistics.hpp>
#include <vap/streaming/frame/frame_exchange.hpp>

class QThread;
class QImage;
namespace vap
{
class StreamingWorker;
class CameraConfig;
class IStreamingService;

class StreamingSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ConnectionState state
               READ state
               NOTIFY stateChanged)

public:
    StreamingSession(QObject* parent = nullptr);
    ~StreamingSession();

    void start(const CameraConfig&);
    void stop();

    ConnectionState state() const;
    const StreamStatistics& statistics() const;

    FrameSnapshot currentFrame() const;

signals:
    void frameUpdated();
    void stateChanged(const ConnectionState&);
    void errorOccurred(const QString& error);
    void statisticsUpdated(const StreamStatistics& statistics);

private slots:
    void onStateChanged(const ConnectionState&);
    void onStatisticsUpdated(const StreamStatistics& statistics);

private:
    std::unique_ptr<QThread> m_streamingThread;
    std::unique_ptr<IStreamingService> m_streamingService;
    std::unique_ptr<StreamingWorker> m_streamingWorker;
    FrameExchange m_frameExchange;
    ConnectionState m_state{ConnectionState::Disconnected};

    StreamStatistics m_statistics;
};

}
