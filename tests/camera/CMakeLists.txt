add_executable(camera_validator_test
    camera_validator_test.cpp
)

target_link_libraries(camera_validator_test
    PRIVATE
        GTest::gtest_main
        camera
)

include(GoogleTest)

gtest_discover_tests(camera_validator_test)
