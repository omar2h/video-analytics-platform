import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../views"
import VAP 1.0

Page {

    Component.onCompleted: {
        cameraManagementViewModel.selectedIndex = 0
    }

    background: Rectangle {
        color: Colors.background
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.pageMargin
        spacing: Metrics.spacingMedium

        Label {
            text: qsTr("Camera Management")
            font.pixelSize: Fonts.title
            color: Colors.textPrimary
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Metrics.spacingMedium

            CameraListPanel {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
            }

            CameraDetailsPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
