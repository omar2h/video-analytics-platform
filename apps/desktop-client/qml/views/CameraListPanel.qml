import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    ColumnLayout {
        anchors.fill: parent

        // Form

        TextField {
            placeholderText: "Camera Name"

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.newCameraName
                  : null

            onTextChanged: cameraManagementViewModel.newCameraName = text
        }

        TextField {
            placeholderText: "Camera URL"

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.newCameraUrl
                  : null

            onTextChanged: cameraManagementViewModel.newCameraUrl = text
        }

        Button {
            text: "Add Camera"

            onClicked: cameraManagementViewModel.addCamera()
        }

        Button {
            text: qsTr("Delete Camera")

            enabled: cameraManagementViewModel.selectedIndex >= 0

            onClicked: cameraManagementViewModel.deleteSelectedCamera()
        }


        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

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
}
