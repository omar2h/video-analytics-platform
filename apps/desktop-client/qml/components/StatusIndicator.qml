import QtQuick

Row {
    id: root
    property int state: 0
    property string stateText: ""

    spacing: 8
    Rectangle{
        width: 12
        height:12
        radius: width/2

        color: {
            switch(root.state) {

            case 0: // Disconnected
                return "gray"

            case 1: // Connecting
                return "orange"

            case 2: // Connected
                return "green"

            case 3: // Reconnecting
                return "orange"

            case 4: // Error
                return "red"

            default:
                return "gray"
            }
        }
    }
    Text {
        text: root.stateText
    }
}
