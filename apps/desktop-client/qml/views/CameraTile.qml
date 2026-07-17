import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Frame {
    id: root

    property string name: ""
    property int state: ConnectionState.Disconnected
    property url imageSource: ""
    property bool hasVideo: false

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
                id: videoImage

                anchors.fill: parent

                visible: root.hasVideo

                fillMode: Image.PreserveAspectFit
                cache: false

                source: root.imageSource
            }

            VideoPlaceholder {
                anchors.fill: parent

                visible: !root.hasVideo

                state: root.state
            }
        }
    }
}
