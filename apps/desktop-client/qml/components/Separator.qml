import QtQuick
import QtQuick.Layouts

import VAP 1.0

Rectangle {
    id: root

    property color lineColor: Colors.border
    property int thickness: 1

    Layout.fillWidth: true
    height: thickness
    color: lineColor
}
