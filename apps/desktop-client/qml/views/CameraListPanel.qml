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
                  ? cameraManagementViewModel.cameraName
                  : null

            onTextChanged: cameraManagementViewModel ? cameraManagementViewModel.cameraName = text : null
        }

        TextField {
            placeholderText: "Camera URL"

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.cameraUrl
                  : null

            onTextChanged: cameraManagementViewModel ? cameraManagementViewModel.cameraUrl = text : null
        }

        Button {
            text: cameraManagementViewModel
                  ? cameraManagementViewModel.selectedIndex >= 0
                        ? "Save Camera"
                        : "Add Camera"
                  : null

            onClicked: {
                cameraManagementViewModel.selectedIndex >= 0 ?
                    cameraManagementViewModel.updateSelectedCamera() : cameraManagementViewModel.addCamera()
            }
        }

        Button {
            text: qsTr("Delete Camera")

            enabled: cameraManagementViewModel ? cameraManagementViewModel.selectedIndex >= 0 ? true : false : false

            onClicked: cameraManagementViewModel.deleteSelectedCamera()
        }

        Button {
            text: "Cancel"

            enabled: cameraManagementViewModel ? cameraManagementViewModel.selectedIndex >= 0 ? true : false : false

            onClicked: cameraManagementViewModel.clearSelection()
        }

        Button {
            text: "Connect"

            enabled: cameraManagementViewModel ? cameraManagementViewModel.selectedIndex >= 0 ? true : false : false

            onClicked: cameraManagementViewModel.connectSelectedCamera()
        }

        Text {
            text: cameraManagementViewModel ? cameraManagementViewModel.validationMessage ? cameraManagementViewModel.validationMessage : "" : ""
            visible: text.length > 0
            color: "red"
        }

        CameraTile {
            Layout.fillWidth: true

            state: cameraStreamViewModel.state
            frameRevision: cameraStreamViewModel.frameRevision
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
