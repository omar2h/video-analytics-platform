add_library(camera STATIC

    include/vap/camera/camera.hpp
    include/vap/camera/camera_config.hpp
    include/vap/camera/i_camera_service.hpp
    include/vap/camera/mock_camera_service.hpp
    include/vap/camera/repositories/i_camera_repository.hpp
    include/vap/camera/repositories/in_memory_camera_repository.hpp
    include/vap/camera/services/i_camera_application_service.hpp
    include/vap/camera/services/camera_application_service.hpp
    include/vap/camera/validation/camera_validation.hpp
    include/vap/camera/validation/i_camera_validator.hpp
    include/vap/camera/validation/camera_validator.hpp

    src/mock_camera_service.cpp
    src/repositories/in_memory_camera_repository.cpp
    src/services/camera_application_service.cpp
    src/validation/camera_validator.cpp
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
