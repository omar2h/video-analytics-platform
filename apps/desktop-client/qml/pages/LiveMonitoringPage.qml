import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16

        Label {
            text: qsTr("Live Monitoring")
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                anchors.centerIn: parent
                text: qsTr("Monitoring dashboard coming soon")
            }
        }
    }
}
