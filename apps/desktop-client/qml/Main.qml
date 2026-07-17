import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "pages"

ApplicationWindow {
    visible: true
    width: 1400
    height: 900

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Camera Management")
            }

            TabButton {
                text: qsTr("Live Monitoring")
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: tabBar.currentIndex

            CameraManagementPage { }

            LiveMonitoringPage { }
        }
    }
}
