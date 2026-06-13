import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    readonly property var vm: cameraManagementViewModel

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: "Camera Details"
            font.bold: true
        }

        Label {
            text: "Name:"
        }

        Label {
            text: vm ? vm.selectedCameraName : ""
        }

        Label {
            text: "URL:"
        }

        Label {
            text: vm ? vm.selectedCameraUrl : ""
        }
    }
}
