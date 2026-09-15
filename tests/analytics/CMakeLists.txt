add_executable(analytics_postprocessing_test
    yolox_postprocessing_test.cpp

    ${PROJECT_SOURCE_DIR}/modules/analytics/src/detection/onnx/yolox_postprocessing.cpp
)

target_compile_features(analytics_postprocessing_test
    PRIVATE cxx_std_20
)

target_include_directories(analytics_postprocessing_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/modules/analytics/include
        ${PROJECT_SOURCE_DIR}/modules/analytics/src/detection/onnx
)

target_link_libraries(analytics_postprocessing_test
    PRIVATE GTest::gtest_main
)

set_target_properties(analytics_postprocessing_test PROPERTIES
    CXX_EXTENSIONS OFF
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)

include(GoogleTest)

gtest_discover_tests(analytics_postprocessing_test)

add_executable(analytics_session_test
    analytics_session_test.cpp
)

target_link_libraries(analytics_session_test
    PRIVATE
        GTest::gtest_main
        analytics_qt
)

set_target_properties(analytics_session_test PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
    CXX_EXTENSIONS OFF
)

gtest_discover_tests(analytics_session_test
    PROPERTIES TIMEOUT 15
)
