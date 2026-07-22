import QtQuick

import VAP 1.0

Rectangle {
    id: root

    default property alias content: contentItem.data

    color: Colors.surface
    border.color: Colors.border
    radius: Metrics.radiusMedium

    clip: true

    Item {
        id: contentItem
        anchors.fill: parent
    }
}
