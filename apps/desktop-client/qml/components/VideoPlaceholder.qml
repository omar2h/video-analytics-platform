import QtQuick
import QtQuick.Controls

Rectangle {
    color: "#202020"
    border.color: "#404040"
    border.width: 1
    radius: 4

    Label {
        anchors.centerIn: parent

        text: qsTr("No Video")
        color: "#A0A0A0"
    }
}
