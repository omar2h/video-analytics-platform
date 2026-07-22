import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import VAP 1.0

Rectangle {
    id: root

    required property int state

    readonly property string placeholderText: {
        switch (state) {
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

    color: Colors.background
    radius: Metrics.radiusSmall

    ColumnLayout {
        anchors.centerIn: parent
        spacing: Metrics.spacingSmall

        Label {
            Layout.alignment: Qt.AlignHCenter

            text: root.placeholderText

            color: Colors.textSecondary
            font.pixelSize: Fonts.body
        }
    }
}
