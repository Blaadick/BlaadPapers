import QtQuick
import QtQuick.Controls

MenuItem {
    id: control

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: control.highlighted ? "#3a3a3a" : "transparent"
        radius: 10
    }
}
