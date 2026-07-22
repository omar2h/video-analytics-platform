import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

import VAP 1.0

Card {
    id: root

    required property string name
    required property int state
    required property url imageSource
    required property bool hasVideo

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Metrics.panelPadding
        spacing: Metrics.spacingMedium

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 44

            color: Colors.surfaceVariant
            radius: Metrics.radiusSmall

            RowLayout {
                anchors.fill: parent
                anchors.margins: Metrics.panelPadding

                Label {
                    Layout.fillWidth: true

                    text: root.name

                    color: Colors.textPrimary
                    font.pixelSize: Fonts.body
                    font.bold: true

                    elide: Text.ElideRight
                }

                StatusIndicator {
                    state: root.state
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            radius: Metrics.radiusSmall
            color: Colors.background

            clip: true

            Image {
                id: videoImage

                anchors.fill: parent

                visible: root.hasVideo

                fillMode: Image.PreserveAspectFit
                cache: false

                source: root.imageSource
            }

            VideoPlaceholder {
                anchors.fill: parent

                visible: !root.hasVideo

                state: root.state
            }
        }
    }
}
