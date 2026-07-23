qt_add_executable(desktop_client
    src/main.cpp

    src/bootstrap/application_bootstrap.hpp
    src/bootstrap/application_bootstrap.cpp

    src/models/camera_list_model.cpp
    src/models/camera_list_model.hpp

    src/viewmodels/camera_management_view_model.cpp
    src/viewmodels/camera_management_view_model.hpp

    src/viewmodels/camera_stream_view_model.hpp
    src/viewmodels/camera_stream_view_model.cpp

    src/viewmodels/live_monitoring_view_model.hpp
    src/viewmodels/live_monitoring_view_model.cpp

    src/providers/video_frame_provider.hpp
    src/providers/video_frame_provider.cpp
)

set_source_files_properties(
    qml/style/Colors.qml
    qml/style/Metrics.qml
    qml/style/Fonts.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(desktop_client
    URI VAP
    VERSION 1.0

    QML_FILES
        qml/Main.qml

        qml/components/VideoPlaceholder.qml
        qml/components/StatusIndicator.qml
        qml/components/Card.qml
        qml/components/Separator.qml

        qml/pages/CameraManagementPage.qml
        qml/pages/LiveMonitoringPage.qml

        qml/views/CameraTile.qml
        qml/views/CameraGrid.qml
        qml/views/CameraListPanel.qml
        qml/views/CameraDetailsPanel.qml

        qml/style/Colors.qml
        qml/style/Metrics.qml
        qml/style/Fonts.qml
)

target_link_libraries(desktop_client
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        streaming
        camera
)
