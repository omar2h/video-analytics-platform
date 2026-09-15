#pragma once

#include <vap/streaming/services/i_streaming_service.hpp>
#include <stop_token>

namespace vap
{

class MockStreamingService final : public IStreamingService
{
    Q_OBJECT

public:
    explicit MockStreamingService(QObject* parent = nullptr);

    StreamingExitReason stream(const QString& uri, std::stop_token stopToken) override;

};

}
