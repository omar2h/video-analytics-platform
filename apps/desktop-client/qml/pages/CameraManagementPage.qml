import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../views"

Page {

    Component.onCompleted: {
        cameraManagementViewModel.selectedIndex = 0
    }

    RowLayout {
        anchors.fill: parent

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
