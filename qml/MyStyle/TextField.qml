import QtQuick
import QtQuick.Controls

TextField {
    id: control

    implicitWidth: 90
    implicitHeight: 30

    background: Rectangle {
        anchors.fill: parent
        radius: 10
        color: Qt.rgba(0, 0, 0, 0.3)
    }
}
