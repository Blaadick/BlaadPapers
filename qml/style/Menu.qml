import QtQuick
import QtQuick.Controls
import BlaadPapers as Style

Menu {
    id: control

    delegate: Style.MenuItem {}

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40

        color: "#2d2d2d"
        radius: 10
    }
}
