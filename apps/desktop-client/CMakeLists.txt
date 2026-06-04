qt_add_executable(desktop_client
    src/main.cpp
)

qt_add_qml_module(desktop_client
    URI videoanalytics
    VERSION 1.0

    QML_FILES
        qml/Main.qml
)

target_link_libraries(desktop_client
    PRIVATE
        Qt6::Quick
)
