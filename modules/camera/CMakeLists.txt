add_library(camera STATIC

    include/vap/camera/camera.hpp
    include/vap/camera/camera_config.hpp
    include/vap/camera/i_camera_service.hpp
    include/vap/camera/mock_camera_service.hpp

    src/mock_camera_service.cpp
)

target_include_directories(camera
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(camera
    PUBLIC
        common
        Qt6::Core
)
