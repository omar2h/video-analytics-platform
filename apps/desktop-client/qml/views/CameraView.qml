import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Page {
    id: root
    title: "Camera"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Camra URL"
        }

        TextField {
            Layout.fillWidth: true
            placeholderText: "rtsp://camera/live"
            text: cameraViewModel.url
            onTextChanged:
                cameraViewModel.url = text
        }

        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "Connect"

                onClicked:
                    cameraViewModel.connectCamera()
            }

            Button {
                text: "Disconnect"

                onClicked:
                    cameraViewModel.disconnectCamera()
            }
        }

        StatusIndicator {
            state: cameraViewModel.state
            stateText: cameraViewModel.stateText
        }

        VideoPlaceholder {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumHeight: 300
        }
    }
}

