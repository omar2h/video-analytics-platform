add_library(common STATIC

    include/vap/common/stream_state.hpp
    include/vap/common/types.hpp
    include/vap/common/expected.hpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
