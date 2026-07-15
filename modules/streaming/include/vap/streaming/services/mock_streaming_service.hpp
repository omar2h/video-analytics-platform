#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>

namespace vap
{

class MockStreamingService final : public IStreamingService
{
    Q_OBJECT

public:
    explicit MockStreamingService(QObject* parent = nullptr);

    StreamingExitReason stream(const QString& uri) override;

    void requestCancellation() override;
};

}
