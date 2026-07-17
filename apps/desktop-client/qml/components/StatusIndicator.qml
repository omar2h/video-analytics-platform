import QtQuick
import QtQuick.Controls

import VAP 1.0

Row {
    id: root

    property int state: ConnectionState.Disconnected

    spacing: 8

    Rectangle {
        width: 12
        height: 12
        radius: width / 2

        color: {
            switch (root.state) {
            case ConnectionState.Disconnected:
                return "gray"

            case ConnectionState.Connecting:
                return "orange"

            case ConnectionState.Connected:
                return "green"

            case ConnectionState.Reconnecting:
                return "orange"

            case ConnectionState.Error:
                return "red"

            default:
                return "gray"
            }
        }
    }

    Label {
        text: {
            switch (root.state) {
            case ConnectionState.Disconnected:
                return qsTr("Disconnected")

            case ConnectionState.Connecting:
                return qsTr("Connecting")

            case ConnectionState.Connected:
                return qsTr("Connected")

            case ConnectionState.Reconnecting:
                return qsTr("Reconnecting")

            case ConnectionState.Error:
                return qsTr("Error")

            default:
                return qsTr("Unknown")
            }
        }

        color: "#CFCFCF"
    }
}
