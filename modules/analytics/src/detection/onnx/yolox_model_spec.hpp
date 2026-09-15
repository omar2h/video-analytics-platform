#pragma once

#include <array>
#include <cstddef>

namespace vap::detail
{

inline constexpr int InputWidth = 416;
inline constexpr int InputHeight = 416;

inline constexpr std::size_t ClassCount = 80;
inline constexpr std::size_t ValuesPerCandidate = 5 + ClassCount;

inline constexpr std::array<int, 3> Strides{8, 16, 32};

inline constexpr std::size_t CandidateCount = []
{
    std::size_t count = 0;

    for (const int stride : Strides)
    {
        count += static_cast<std::size_t>(InputWidth / stride)
                 * (InputHeight / stride);
    }

    return count;
}();

static_assert(CandidateCount == 3549);

} // namespace vap::detail
