#pragma once

#include <QObject>
#include <memory>

#include <vap/common/connection_state.hpp>

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

signals:
    void frameReady(const QImage&);
    void stateChanged(const ConnectionState&);
    void errorOccurred(const QString& error);

private slots:
    void onStateChanged(const ConnectionState&);

private:
    std::unique_ptr<QThread> m_streamingThread;
    std::unique_ptr<IStreamingService> m_streamingService;
    std::unique_ptr<StreamingWorker> m_streamingWorker;
    ConnectionState m_state{ConnectionState::Disconnected};
};

}
