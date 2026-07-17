import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Frame {
    id: root

    property string name: ""
    property int state: ConnectionState.Disconnected
    property url imageSource: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        Label {
            Layout.fillWidth: true

            text: root.name
            font.bold: true
            font.pixelSize: 16
        }

        StatusIndicator {
            state: root.state
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Image {
                anchors.fill: parent

                visible: root.imageSource.toString() !== ""

                fillMode: Image.PreserveAspectFit
                cache: false

                source: root.imageSource
            }

            VideoPlaceholder {
                anchors.fill: parent

                visible: root.imageSource.toString() === ""
            }
        }
    }
}
