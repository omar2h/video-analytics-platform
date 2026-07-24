import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"
import VAP 1.0

Card {
    readonly property bool hasSelection:
        cameraManagementViewModel
        && cameraManagementViewModel.selectedIndex >= 0

    Connections {
        target: cameraManagementViewModel

        function onFocusCameraNameRequested() {
            cameraNameField.forceActiveFocus()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.panelPadding
        spacing: Metrics.spacingMedium

        Label {
            text: qsTr("Camera Management")

            font.pixelSize: Fonts.sectionTitle
            font.bold: true

            color: Colors.textPrimary
        }

        Separator {}

        Label {
            text: hasSelection
                    ? qsTr("Edit Camera")
                    : qsTr("New Camera")
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
            id: cameraNameField

            Layout.fillWidth: true
            placeholderText: qsTr("Camera Name")

            text: cameraManagementViewModel
                  ? cameraManagementViewModel.cameraName
                  : ""

            onTextChanged: {
                if (cameraManagementViewModel)
                    cameraManagementViewModel.cameraName = text
            }
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

            onTextChanged: {
                if (cameraManagementViewModel)
                    cameraManagementViewModel.cameraUrl = text
            }
        }

        Label {
            text: cameraManagementViewModel
                  ? cameraManagementViewModel.validationMessage
                  : ""
            visible: text.length > 0
            color: Colors.error
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

        Separator {}

        Button {
            Layout.fillWidth: true
            text: qsTr("Delete Camera")

            enabled: hasSelection

            onClicked: cameraManagementViewModel.deleteSelectedCamera()
        }

        Separator {}

        Label {
            text: qsTr("Streaming")
            font.bold: true
        }

        Button {
            Layout.fillWidth: true

            enabled: hasSelection

            text: {
                switch (cameraManagementViewModel.selectedCameraState) {

                case ConnectionState.Connecting:
                case ConnectionState.Connected:
                case ConnectionState.Reconnecting:
                    return qsTr("Stop")

                default:
                    return qsTr("Connect")
                }
            }

            onClicked: {
                switch (cameraManagementViewModel.selectedCameraState) {

                case ConnectionState.Connecting:
                case ConnectionState.Connected:
                case ConnectionState.Reconnecting:
                    cameraManagementViewModel.stopSelectedCamera()
                    break

                default:
                    cameraManagementViewModel.connectSelectedCamera()
                    break
                }
            }
        }

        Separator {}

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: qsTr("Camera List")
                font.bold: true

                Layout.fillWidth: true
            }

            Button {
                text: qsTr("+")

                onClicked: {
                    cameraManagementViewModel.beginAddCamera()
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: Metrics.spacingSmall
            clip: true

            model: cameraManagementViewModel
                   ? cameraManagementViewModel.cameraModel
                   : null

            delegate: Rectangle {
                required property int index
                required property string name
                required property string url

                width: ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin
                height: 72

                radius: Metrics.radiusSmall

                color: cameraManagementViewModel.selectedIndex === index
                       ? Colors.primary
                       : Colors.surfaceVariant

                border.color: Colors.border
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Metrics.panelPadding
                    spacing: 4

                    Label {
                        Layout.fillWidth: true

                        text: name

                        color: Colors.textPrimary
                        font.pixelSize: Fonts.body
                        font.bold: true

                        elide: Text.ElideRight
                    }

                    Label {
                        Layout.fillWidth: true

                        text: url

                        color: Colors.textSecondary
                        font.pixelSize: Fonts.caption

                        elide: Text.ElideMiddle
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: cameraManagementViewModel.selectedIndex = index
                }
            }
        }
    }
}
