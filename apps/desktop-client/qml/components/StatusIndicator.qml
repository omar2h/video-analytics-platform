import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import VAP 1.0

RowLayout {
    id: root

    required property int state

    readonly property color indicatorColor: {
        switch (state) {
        case ConnectionState.Disconnected:
            return Colors.textSecondary

        case ConnectionState.Connecting:
        case ConnectionState.Reconnecting:
            return Colors.warning

        case ConnectionState.Connected:
            return Colors.success

        case ConnectionState.Error:
            return Colors.error

        default:
            return Colors.textSecondary
        }
    }

    readonly property string statusText: {
        switch (state) {
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


    spacing: Metrics.spacingSmall

    Rectangle {
        Layout.alignment: Qt.AlignVCenter

        width: Metrics.statusIndicatorSize
        height: Metrics.statusIndicatorSize
        radius: width / 2

        color: root.indicatorColor
    }

    Label {
        Layout.alignment: Qt.AlignVCenter

        text: root.statusText

        color: Colors.textSecondary
        font.pixelSize: Fonts.caption
    }
}
