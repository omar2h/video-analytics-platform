import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

import VAP 1.0

Card {
    property var cameraVm
    property var streamVm

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
                text: cameraVm ? cameraVm.selectedCameraName : ""
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("RTSP URL")
                color: Colors.textSecondary
            }

            Label {
                text: cameraVm ? cameraVm.selectedCameraUrl : ""
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
                state: streamVm
                    ? streamVm.state
                    : ConnectionState.Disconnected
            }
        }

        Separator {}

        Label {
            text: qsTr("Stream")
            font.bold: true
        }

        GridLayout {
            Layout.fillWidth: true

            columns: 2
            rowSpacing: Metrics.spacingSmall
            columnSpacing: Metrics.spacingMedium

            Label {
                text: qsTr("Resolution")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm ? streamVm.resolution : "—"
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("Codec")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm && streamVm.codec !== ""
                        ? streamVm.codec
                        : "—"
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("FPS")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm && streamVm.fps > 0
                        ? Number(streamVm.fps).toFixed(1)
                        : "—"
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("Bitrate")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm && streamVm.bitrateMbps > 0
                        ? Number(streamVm.bitrateMbps).toFixed(2) + " Mbps"
                        : "—"
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("Frames")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm ? streamVm.framesDecoded : "0"
                color: Colors.textPrimary
            }

            Label {
                text: qsTr("Packets")
                color: Colors.textSecondary
            }

            Label {
                text: streamVm ? streamVm.packetsReceived : "0"
                color: Colors.textPrimary
            }
        }
    }
}
