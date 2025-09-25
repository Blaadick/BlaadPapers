import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: layout.width
    height: layout.height

    RowLayout {
        id: layout
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Label {
            id: info
            text: `${StatusModel.statusText}${StatusModel.repeatCount > 1 ? ` x${StatusModel.repeatCount}` : ''}`
        }
    }
}
