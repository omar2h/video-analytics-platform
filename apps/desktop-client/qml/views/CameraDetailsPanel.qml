import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

import VAP 1.0

Card {
    readonly property var vm: cameraManagementViewModel

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.panelPadding
        spacing: Metrics.spacingSmall

        Label {
            text: qsTr("Camera Details")
            font.pixelSize: Fonts.sectionTitle
            font.bold: true
            color: Colors.textPrimary
        }

        Separator {}

        Label {
            text: qsTr("General")
            font.bold: true
        }

        Separator {}

        GridLayout {
            Layout.fillWidth: true

            columns: 2
            rowSpacing: Metrics.spacingSmall
            columnSpacing: Metrics.spacingMedium

            Label {
                text: qsTr("Name")
                color: Colors.textSecondary
            }

            Label {
                text: vm ? vm.selectedCameraName : ""
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("RTSP URL")
                color: Colors.textSecondary
            }

            Label {
                text: vm ? vm.selectedCameraUrl : ""
                color: Colors.textPrimary
                wrapMode: Text.Wrap
            }
        }

        Separator {}

        Label {
            text: qsTr("Connection")
            font.bold: true
        }

        GridLayout {
            Layout.fillWidth: true

            columns: 2
            rowSpacing: Metrics.spacingSmall
            columnSpacing: Metrics.spacingMedium

            Label {
                text: qsTr("Status")
                color: Colors.textSecondary
            }

            StatusIndicator {
                state: vm
                    ? vm.selectedCameraState
                    : ConnectionState.Disconnected
            }
        }

        Separator {}

        Label {
            text: qsTr("Stream")
            font.bold: true
        }

        Label {
            text: qsTr("Resolution")
            color: Colors.textSecondary
        }

        Label {
            text: "—"
        }

        Label {
            text: qsTr("FPS")
            color: Colors.textSecondary
        }

        Label {
            text: "—"
        }

        Label {
            text: qsTr("Codec")
            color: Colors.textSecondary
        }

        Label {
            text: "—"
        }

        Label {
            text: qsTr("Recording")
            color: Colors.textSecondary
        }

        Label {
            text: "No"
        }
    }
}
