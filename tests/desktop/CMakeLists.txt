set(DESKTOP_SOURCE_DIR
    "${PROJECT_SOURCE_DIR}/apps/desktop-client"
)

add_executable(session_removal_test
    session_removal_test.cpp

    "${DESKTOP_SOURCE_DIR}/src/viewmodels/camera_stream_view_model.hpp"
    "${DESKTOP_SOURCE_DIR}/src/viewmodels/camera_stream_view_model.cpp"

    "${DESKTOP_SOURCE_DIR}/src/viewmodels/live_monitoring_view_model.hpp"
    "${DESKTOP_SOURCE_DIR}/src/viewmodels/live_monitoring_view_model.cpp"

    "${DESKTOP_SOURCE_DIR}/src/providers/video_frame_provider.hpp"
    "${DESKTOP_SOURCE_DIR}/src/providers/video_frame_provider.cpp"

    "${DESKTOP_SOURCE_DIR}/src/models/detection_list_model.hpp"
    "${DESKTOP_SOURCE_DIR}/src/models/detection_list_model.cpp"
)

target_compile_features(session_removal_test PRIVATE cxx_std_20)

target_include_directories(session_removal_test
    PRIVATE
        "${DESKTOP_SOURCE_DIR}"
)

target_link_libraries(session_removal_test
    PRIVATE
        GTest::gtest_main
        Qt6::Quick
        streaming
)

set_target_properties(session_removal_test PROPERTIES
    AUTOMOC ON
    AUTOUIC OFF
    AUTORCC OFF
    CXX_EXTENSIONS OFF
)

include(GoogleTest)

gtest_discover_tests(session_removal_test
    PROPERTIES TIMEOUT 20
)
