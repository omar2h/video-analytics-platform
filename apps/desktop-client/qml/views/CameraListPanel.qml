import QtQuick
import QtQuick.Controls

Frame {

    ListView {
        anchors.fill: parent

        model: cameraManagementViewModel
               ? cameraManagementViewModel.cameraModel
               : null

        delegate: Rectangle {

            required property int index
            required property string name

            color: cameraManagementViewModel.selectedIndex === index
                   ? "#d0d0d0"
                   : "white"

            width: ListView.view.width
            height: 50

            border.width: 1

            Text {
                anchors.centerIn: parent
                text: name
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    cameraManagementViewModel.selectedIndex = index
                }
            }
        }
    }
}
