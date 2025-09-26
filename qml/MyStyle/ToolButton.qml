import QtQuick
import QtQuick.Controls

ToolButton {
    id: control

    implicitWidth: 30
    implicitHeight: 30
    scale: pressed ? 0.97 : (hovered ? 1.03 : 1)

    background: Rectangle {
        anchors.fill: parent
        radius: 10
        color: Qt.rgba(0, 0, 0, 0.3)
    }

    Behavior on scale { ScaleAnim {} }
}
