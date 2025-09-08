import QtCore
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import BlaadPapers

Image {
    id: preview
    source: `${StandardPaths.writableLocation(StandardPaths.CacheLocation)}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}/${name}.png`
    scale: isPressed ? 0.97 : (isHovered ? 1.03 : 1)
    layer {
        enabled: true
        smooth: true
        mipmap: true
        effect: OpacityMask {
            maskSource: Rectangle {
                width: preview.width
                height: preview.height
                radius: 10
            }
        }
    }
    fillMode: Image.PreserveAspectCrop
    asynchronous: true

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.OutQuad
            duration: 60
        }
    }

    property string name
    property string description
    property string picturePath
    property bool isPressed: false
    property bool isHovered: false

    ToolTip {
        id: tooltip
        text: `${name}${description === "" ? "" : `\n${description}`}`
    }

    MouseArea {
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        anchors.fill: parent

        onPressed: {
            preview.isPressed = true
        }

        onReleased: {
            preview.isPressed = false
            WallpaperList.setWallpaper(picturePath)
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
