add_executable(analytics_demo
    src/main.cpp
)

target_link_libraries(analytics_demo
    PRIVATE
        analytics
        opencv_imgcodecs
)

set_target_properties(analytics_demo PROPERTIES
    BUILD_RPATH "${ONNXRUNTIME_ROOT}/lib"
    CXX_EXTENSIONS OFF
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
