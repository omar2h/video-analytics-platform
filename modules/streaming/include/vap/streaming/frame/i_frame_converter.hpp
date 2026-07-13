#pragma once

#include <QImage>

struct AVFrame;

namespace vap
{

class IFrameConverter
{
public:
    virtual ~IFrameConverter() = default;

    [[nodiscard]]
    virtual QImage convert(const AVFrame* frame) = 0;
};

}
