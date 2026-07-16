import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame
{
    id: root

    property string name: ""
    property int state: 0
    property string stateText: ""
    property int frameRevision: 0

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Label {
            text: root.name
            font.bold: true
        }

        StatusIndicator {
            state: root.state
            stateText: root.stateText
        }

        Image {
            Layout.fillWidth: true
            Layout.preferredHeight: 360

            fillMode: Image.PreserveAspectFit
            cache: false

            source: "image://video/current?rev="
                    + cameraStreamViewModel.frameRevision
        }
    }
}


