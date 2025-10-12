// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtCore
import QtQuick
import QtQuick.Controls

Item {
    id: preview
    scale: isPressed || !isHovered ? 1 : 1.06

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

    AnimatedImage {
        anchors.fill: parent
        visible: censor.opacity < 1
        source: `${StandardPaths.writableLocation(StandardPaths.CacheLocation)}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}/${preview.wid}.webp`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
    }

    Rectangle {
        id: censor

        anchors.fill: parent
        opacity: preview.isBad && !preview.isHovered ? 1 : 0
        color: "black"

        Text {
            anchors.centerIn: parent
            text: "BAD"
            font.pixelSize: parent.width * 0.5
            font.bold: true
            color: "darkred"
        }

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.OutQuad
                duration: 300
            }
        }
    }

    ToolTip {
        id: tooltip
        text: `${preview.name}\n${preview.tags.join(", ")}`
    }

    Menu {
        id: contextMenu

        Action {
            text: "Apply"
            onTriggered: Wallpapers.applyWallpaper(preview.wid)
        }

        Action {
            text: "Edit"
            icon.name: "editor"
        }

        MenuSeparator {}

        Action {
            text: "Delete"
            icon.name: "delete"
            onTriggered: Wallpapers.deleteWallpaper(preview.wid)
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
                Wallpapers.applyWallpaper(preview.wid)
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
