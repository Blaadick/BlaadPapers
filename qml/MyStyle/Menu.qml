import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import BlaadPapers as S

Menu {
    id: control

    delegate: S.MenuItem {}

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        radius: 10
        color: Qt.rgba(0, 0, 0, 0.3)
    }

    MultiEffect {}
}
