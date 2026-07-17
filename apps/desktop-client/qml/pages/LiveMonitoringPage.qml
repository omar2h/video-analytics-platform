import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import VAP 1.0

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
                id: tile

                property var stream:
                    liveMonitoringViewModel.streamViewModel(model.id)

                width: cameraGrid.cellWidth - 16
                height: cameraGrid.cellHeight - 16

                name: model.name

                state: stream
                       ? stream.state
                       : ConnectionState.Disconnected
                imageSource: stream
                    ? "image://video/" + model.id + "?rev=" + stream.frameRevision
                    : ""
            }

            ScrollBar.vertical: ScrollBar { }
        }
    }
}
