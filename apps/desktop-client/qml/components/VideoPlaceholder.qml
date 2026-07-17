import QtQuick
import QtQuick.Controls

import VAP 1.0

Rectangle {
    color: "#202020"
    border.color: "#404040"
    border.width: 1
    radius: 4

    Label {
        anchors.centerIn: parent

        text: {
            switch (root.state) {
            case ConnectionState.Connecting:
                return qsTr("Connecting...")

            case ConnectionState.Reconnecting:
                return qsTr("Reconnecting...")

            case ConnectionState.Disconnected:
                return qsTr("Disconnected")

            default:
                return qsTr("No Video")
            }
        }
        color: "#A0A0A0"
    }
}
