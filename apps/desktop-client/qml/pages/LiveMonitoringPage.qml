import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../views"

Page {
    id: root

    title: qsTr("Live Monitoring")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Label {
            text: qsTr("Live Monitoring")
            font.pixelSize: 24
            font.bold: true
        }

        GridView {
            id: cameraGrid

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            cellWidth: 380
            cellHeight: 320

            model: cameraManagementViewModel.cameraModel

            delegate: CameraTile {
                width: cameraGrid.cellWidth - 16
                height: cameraGrid.cellHeight - 16

                name: model.name

                // Temporary values.
                // These will be connected to StreamingManager
                // in the next milestone.
                state: 0
                imageSource: ""
            }

            ScrollBar.vertical: ScrollBar { }
        }
    }
}
