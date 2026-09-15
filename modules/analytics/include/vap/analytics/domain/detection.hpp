#pragma once

namespace vap
{

struct BoundingBox
{
    double x{};
    double y{};
    double width{};
    double height{};
};

struct Detection
{
    BoundingBox box;
    int classId{};
    float score{};
};

} // namespace vap
