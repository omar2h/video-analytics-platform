cmake_minimum_required(VERSION 3.16)

project(video-analytics-platform
    VERSION 0.1
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    QuickControls2
    Sql
)
find_package(PkgConfig REQUIRED)

qt_standard_project_setup()

add_subdirectory(modules/common)
add_subdirectory(modules/camera)
add_subdirectory(modules/streaming)
add_subdirectory(modules/database)
add_subdirectory(apps/desktop-client)
add_subdirectory(tests)

