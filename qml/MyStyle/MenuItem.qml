import QtQuick
import QtQuick.Controls

MenuItem {
    id: control

    implicitWidth: 200
    implicitHeight: 40

    background: Rectangle {
        anchors.fill: parent
        radius: 10
        color: control.down ? control.palette.midlight : control.highlighted ? Qt.rgba(1, 1, 1, 0.3) : "transparent"
    }
}
