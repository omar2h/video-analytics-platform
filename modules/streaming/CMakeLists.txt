add_library(streaming STATIC
    include/vap/streaming/services/i_streaming_service.hpp
    include/vap/streaming/services/ffmpeg/ffmpeg_streaming_service.hpp
    include/vap/streaming/services/mock_streaming_service.hpp
    include/vap/streaming/frame/i_frame_converter.hpp
    include/vap/streaming/frame/ffmpeg_frame_converter.hpp
    include/vap/streaming/worker/streaming_worker.hpp
    include/vap/streaming/logging.hpp

    src/services/ffmpeg/ffmpeg_streaming_service.cpp
    src/services/mock_streaming_service.cpp
    src/frame/ffmpeg_frame_converter.cpp
    src/worker/streaming_worker.cpp
)

pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)

target_include_directories(streaming
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(streaming
    PUBLIC
        common
        Qt6::Core
        Qt6::Gui

    PRIVATE
        PkgConfig::AVFORMAT
        PkgConfig::AVCODEC
        PkgConfig::AVUTIL
        PkgConfig::SWSCALE
)
