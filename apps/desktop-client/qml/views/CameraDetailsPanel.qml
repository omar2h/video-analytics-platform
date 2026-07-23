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
            text: qsTr("Name")
            color: Colors.textSecondary
            font.pixelSize: Fonts.caption
        }

        Label {
            text: vm ? vm.selectedCameraName : ""
            color: Colors.textPrimary
            font.pixelSize: Fonts.body
        }

        Label {
            text: qsTr("URL")
            color: Colors.textSecondary
            font.pixelSize: Fonts.caption
        }

        Label {
            text: vm ? vm.selectedCameraUrl : ""
            color: Colors.textPrimary
            font.pixelSize: Fonts.body
            wrapMode: Text.Wrap
        }

        Separator {}

        Label {
            text: qsTr("Status")
            color: Colors.textSecondary
            font.pixelSize: Fonts.caption
        }

        StatusIndicator {
            state: vm
                       ? vm.selectedCameraState
                       : ConnectionState.Disconnected
        }
    }
}
