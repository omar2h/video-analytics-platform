import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

import VAP 1.0

Card {
    id: root

    required property string cameraName
    required property int state
    required property url imageSource
    required property bool hasVideo

    property var detectionModel: null
    property size detectionImageSize: Qt.size(0, 0)

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

                    text: root.cameraName

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

                Item {
                    id: detectionOverlay

                    // Match the actual displayed image, excluding black margins.
                    x: (videoImage.width - videoImage.paintedWidth) / 2
                    y: (videoImage.height - videoImage.paintedHeight) / 2

                    width: videoImage.paintedWidth
                    height: videoImage.paintedHeight

                    clip: true

                    visible: root.hasVideo
                             && videoImage.status === Image.Ready
                             && root.detectionImageSize.width > 0
                             && root.detectionImageSize.height > 0

                    readonly property real scaleX:
                        root.detectionImageSize.width > 0
                            ? width / root.detectionImageSize.width
                            : 0

                    readonly property real scaleY:
                        root.detectionImageSize.height > 0
                            ? height / root.detectionImageSize.height
                            : 0

                    Repeater {
                        model: root.detectionModel

                        delegate: Rectangle {
                            required property real boxX
                            required property real boxY
                            required property real boxWidth
                            required property real boxHeight
                            required property string label
                            required property real confidence

                            x: boxX * detectionOverlay.scaleX
                            y: boxY * detectionOverlay.scaleY
                            width: boxWidth * detectionOverlay.scaleX
                            height: boxHeight * detectionOverlay.scaleY

                            color: "transparent"
                            border.color: "#4ade80"
                            border.width: 2

                            Rectangle {
                                // Keep the label inside the box's top edge.
                                x: 0
                                y: 0
                                width: detectionText.implicitWidth + 8
                                height: detectionText.implicitHeight + 4

                                color: "#CC102018"

                                Text {
                                    id: detectionText

                                    anchors.centerIn: parent

                                    text: label + " "
                                          + Math.round(confidence * 100) + "%"

                                    color: "white"
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                }
            }

            VideoPlaceholder {
                anchors.fill: parent

                visible: !root.hasVideo

                state: root.state
            }
        }
    }
}
