import QtCore
import QtQuick
import QtQuick.Controls

Item {
    id: preview
    scale: isPressed ? 0.97 : (isHovered ? 1.03 : 1)

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.OutQuad
            duration: 60
        }
    }

    property string wid
    property string name
    property var tags
    property bool isAnimated
    property bool isBad
    property bool isPressed
    property bool isHovered

    Image {
        anchors.fill: parent
        visible: !preview.isBad || preview.isHovered
        source: `${StandardPaths.writableLocation(StandardPaths.CacheLocation)}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}/${preview.wid}.webp`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
    }

    Rectangle {
        anchors.fill: parent
        visible: preview.isBad && !preview.isHovered
        color: "black"

        Text {
            anchors.centerIn: parent
            text: "BAD"
            font.pixelSize: parent.width * 0.5
            font.bold: true
            color: "darkred"
        }
    }

    ToolTip {
        id: tooltip
        text: `${preview.name} (${preview.wid})\n${preview.tags.join(", ")}`
    }

    Menu {
        id: contextMenu

        Action {
            text: "Apply"
            onTriggered: WallpapersModel.applyWallpaper(preview.wid)
        }

        MenuSeparator {}

        Action {
            text: "Delete"
            onTriggered: WallpapersModel.deleteWallpaper(preview.wid)
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onPressed: (event) => {
            if(event.button === Qt.LeftButton) {
                preview.isPressed = true
            } else {
                contextMenu.popup()
            }
        }

        onReleased: (event) => {
            if(event.button === Qt.LeftButton) {
                preview.isPressed = false
                WallpapersModel.applyWallpaper(preview.wid)
            }
        }

        onCanceled: {
            preview.isPressed = false
        }

        onEntered: {
            preview.isHovered = true
            tooltip.visible = true
        }

        onExited: {
            preview.isHovered = false
            tooltip.visible = false
        }
    }
}
