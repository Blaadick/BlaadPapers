import QtCore
import QtQuick
import QtQuick.Controls
import BlaadPapers

Item {
    id: preview
    scale: isPressed ? 0.97 : (isHovered ? 1.03 : 1)

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.OutQuad
            duration: 60
        }
    }

    property string name
    property string description
    property string picturePath
    property bool isPressed
    property bool isHovered

    Image {
        id: picture
        anchors.fill: parent
        source: `${StandardPaths.writableLocation(StandardPaths.CacheLocation)}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}/${preview.name}.png`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: false
    }

    ToolTip {
        id: tooltip
        text: `${name}${description === "" ? "" : `\n${description}`}`
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true

        onPressed: {
            preview.isPressed = true
        }

        onReleased: {
            preview.isPressed = false
            WallpaperList.setWallpaper(preview.picturePath)
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
