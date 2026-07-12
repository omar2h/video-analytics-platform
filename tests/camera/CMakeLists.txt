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

add_executable(sqlite_camera_repository_test
    sqlite_camera_repository_test.cpp
)

target_link_libraries(sqlite_camera_repository_test
    PRIVATE
        GTest::gtest_main
        camera
        database
)

gtest_discover_tests(sqlite_camera_repository_test)
