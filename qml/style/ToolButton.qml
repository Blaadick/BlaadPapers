import QtQuick
import QtQuick.Controls

ToolButton {
    id: control

    implicitWidth: 30
    implicitHeight: 30
    scale: pressed ? 0.97 : (hovered ? 1.03 : 1)

    background: Rectangle {
        anchors.fill: parent
        color: "#2d2d2d"
        radius: 10
    }

    Behavior on scale { ScaleAnim {} }
}
