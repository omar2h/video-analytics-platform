add_executable(analytics_live_demo
    src/main.cpp
)

target_link_libraries(analytics_live_demo
    PRIVATE
        analytics_qt
        streaming
)

set_target_properties(analytics_live_demo PROPERTIES
    BUILD_RPATH "${ONNXRUNTIME_ROOT}/lib"
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
    CXX_EXTENSIONS OFF
)
