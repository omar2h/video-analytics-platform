import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import VAP 1.0

import "../views"

Page {
    id: root

    background: Rectangle {
        color: Colors.background
    }

    title: qsTr("Live Monitoring")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.pageMargin
        spacing: Metrics.spacingMedium

        Label {
            text: qsTr("Live Monitoring")
            font.pixelSize: Fonts.title
            font.bold: true

            color: Colors.textPrimary
        }

        GridView {
            id: cameraGrid

            Layout.fillWidth: true
            Layout.fillHeight: true

            leftMargin: Metrics.spacingSmall
            rightMargin: Metrics.spacingSmall
            topMargin: Metrics.spacingSmall
            bottomMargin: Metrics.spacingSmall

            clip: true

            cellWidth: Metrics.cameraTileWidth
            cellHeight: Metrics.cameraTileHeight

            boundsBehavior: Flickable.StopAtBounds

            model: cameraManagementViewModel.cameraModel

            delegate: CameraTile {
                required property string id
                required property string name

                property var stream:
                    liveMonitoringViewModel.streamViewModel(id)

                width: cameraGrid.cellWidth - Metrics.spacingMedium
                height: cameraGrid.cellHeight - Metrics.spacingMedium

                cameraName: name

                hasVideo: stream
                    ? stream.hasVideo
                    : false

                state: stream
                       ? stream.state
                       : ConnectionState.Disconnected
                imageSource: stream
                    ? "image://video/" + id + "?rev=" + stream.frameRevision
                    : ""


            }

            ScrollBar.vertical: ScrollBar { }
        }
    }
}
