import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"
import VAP 1.0

Card {
    readonly property bool hasSelection:
        cameraManagementViewModel
        && cameraManagementViewModel.selectedIndex >= 0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.panelPadding
        spacing: Metrics.spacingMedium

        Label {
            text: qsTr("Cameras")

            font.pixelSize: Fonts.sectionTitle
            font.bold: true

            color: Colors.textPrimary
        }

        // Form
        Label {
            text: qsTr("Camera Name")
            color: Colors.textSecondary
            font.pixelSize: Fonts.caption
        }


        TextField {
            Layout.fillWidth: true
            placeholderText: qsTr("Camera Name")

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.cameraName
                  : ""

            onTextChanged: cameraManagementViewModel ? cameraManagementViewModel.cameraName = text : ""
        }

        Label {
            text: qsTr("Camera URL")
            color: Colors.textSecondary
            font.pixelSize: Fonts.caption
        }

        TextField {
            Layout.fillWidth: true
            placeholderText: qsTr("Camera URL")

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.cameraUrl
                  : ""

            onTextChanged: cameraManagementViewModel ? cameraManagementViewModel.cameraUrl = text : ""
        }

        Button {
            text: hasSelection
                    ? qsTr("Save Camera")
                    : qsTr("Add Camera")

            Layout.fillWidth: true

            onClicked: {
                hasSelection ?
                    cameraManagementViewModel.updateSelectedCamera() : cameraManagementViewModel.addCamera()
            }
        }

        Button {
            text: qsTr("Delete Camera")

            Layout.fillWidth: true

            enabled: hasSelection

            onClicked: cameraManagementViewModel.deleteSelectedCamera()
        }

        Button {
            text: qsTr("Cancel")

            Layout.fillWidth: true

            enabled: hasSelection

            onClicked: cameraManagementViewModel.clearSelection()
        }

        Button {
            text: qsTr("Connect")

            Layout.fillWidth: true

            enabled: hasSelection

            onClicked: cameraManagementViewModel.connectSelectedCamera()
        }

        Label {
            text: cameraManagementViewModel ? cameraManagementViewModel.validationMessage ? cameraManagementViewModel.validationMessage : "" : ""
            visible: text.length > 0
            color: Colors.error
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
