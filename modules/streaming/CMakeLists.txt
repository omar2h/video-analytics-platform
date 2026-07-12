add_library(streaming STATIC
    include/vap/streaming/services/i_streaming_service.hpp
    include/vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp
    include/vap/streaming/services/mock_streaming_service.hpp

    src/services/ffmpeg/ffmpeg_streaming_service.cpp
    src/services/mock_streaming_service.cpp
)

target_include_directories(streaming
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(streaming
    PUBLIC
        common
        Qt6::Core
)
