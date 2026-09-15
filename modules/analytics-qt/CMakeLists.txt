find_package(Threads REQUIRED)

add_library(analytics_qt STATIC
    include/vap/analytics/analytics_frame.hpp
    include/vap/analytics/analytics_result.hpp
    include/vap/analytics/analytics_session.hpp

    src/analytics_mailbox.hpp
    src/analytics_result_mailbox.hpp
    src/analytics_worker.hpp
    src/analytics_worker.cpp
    src/analytics_session.cpp
)

target_compile_features(analytics_qt PUBLIC cxx_std_20)

target_include_directories(analytics_qt
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(analytics_qt
    PUBLIC
        analytics
        Qt6::Core
        Qt6::Gui
    PRIVATE
        opencv_imgproc
        Threads::Threads
)

set_target_properties(analytics_qt PROPERTIES
    AUTOMOC ON
    AUTOUIC OFF
    AUTORCC OFF
    CXX_EXTENSIONS OFF
)
