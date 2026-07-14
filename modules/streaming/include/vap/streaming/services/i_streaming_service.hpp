#pragma once

#include <QObject>

#include <vap/common/stream_state.hpp>

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
    virtual void disconnectFromStream() = 0;

signals:
    void stateChanged(StreamState state);
    void frameReady(const QImage& frame);
    void errorOccurred(const QString& error);

};

}
