#pragma once

extern "C"
{
#include <libswscale/swscale.h>
}

#include <vap/streaming/frame/i_frame_converter.hpp>

struct SwsContext;

namespace vap
{

class FFmpegFrameConverter : public IFrameConverter
{
public:
    FFmpegFrameConverter() = default;
    ~FFmpegFrameConverter() override;

    [[nodiscard]]
    QImage convert(const AVFrame* frame) override;

private:
    bool ensureContext(const AVFrame* frame);

private:
    SwsContext* m_swsContext = nullptr;
};

}
