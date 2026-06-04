cmake_minimum_required(VERSION 3.16)

project(video-analytics-platform
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

add_subdirectory(apps/desktop-client)
