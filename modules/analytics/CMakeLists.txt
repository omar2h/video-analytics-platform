set(ONNXRUNTIME_ROOT "" CACHE PATH
    "Path to the extracted ONNX Runtime package"
)

if(NOT EXISTS "${ONNXRUNTIME_ROOT}/include/onnxruntime_cxx_api.h")
    message(FATAL_ERROR
        "Set ONNXRUNTIME_ROOT to the extracted ONNX Runtime directory"
    )
endif()

if(NOT EXISTS "${ONNXRUNTIME_ROOT}/lib/libonnxruntime.so")
    message(FATAL_ERROR
        "Cannot find ONNXRUNTIME_ROOT/lib/libonnxruntime.so"
    )
endif()

add_library(vap_onnxruntime SHARED IMPORTED GLOBAL)

set_target_properties(vap_onnxruntime PROPERTIES
    IMPORTED_LOCATION
        "${ONNXRUNTIME_ROOT}/lib/libonnxruntime.so"
    INTERFACE_INCLUDE_DIRECTORIES
        "${ONNXRUNTIME_ROOT}/include"
)

add_library(analytics STATIC
    include/vap/analytics/domain/detection.hpp
    include/vap/analytics/detection/i_object_detector.hpp
    include/vap/analytics/detection/onnx/onnx_yolox_detector.hpp

    src/detection/onnx/onnx_yolox_detector.cpp
    src/detection/onnx/yolox_model_spec.hpp
    src/detection/onnx/yolox_postprocessing.hpp
    src/detection/onnx/yolox_postprocessing.cpp
)

target_compile_features(analytics PUBLIC cxx_std_20)

target_include_directories(analytics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(analytics SYSTEM
    PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(analytics
    PUBLIC
        opencv_core
    PRIVATE
        opencv_imgproc
        vap_onnxruntime
)

set_target_properties(analytics PROPERTIES
    CXX_EXTENSIONS OFF
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)
