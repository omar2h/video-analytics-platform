add_library(common STATIC

    include/vap/stream_state.hpp
    include/vap/types.hpp
    include/vap/expected.hpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
