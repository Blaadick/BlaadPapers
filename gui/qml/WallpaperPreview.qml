// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: preview

    property string wid
    property string name
    property string rootDir
    property string source
    property var tags
    property bool isBad
    property bool isPressed
    property bool isHovered

    ContextMenu.menu: Menu {
        Action {
            text: "Apply"
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/apply.svg"
            onTriggered: Wallpapers.applyWallpaperAsync(preview.wid)
        }

        Action {
            text: "Open Folder"
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/folder_open.svg"
            onTriggered: Qt.openUrlExternally(`file://${preview.rootDir}`)
        }

        Action {
            text: "Copy Deeplink"
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/link.svg"
            onTriggered: Clipboard.copyWallpaperDeeplink(preview.wid)
        }

        MenuSeparator {}

        MenuItem {
            DelayButton {
                text: "Delete"
                icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/delete.svg"
                anchors.fill: parent

                onActivated: {
                    Wallpapers.deleteWallpaperAsync(preview.wid)
                    contextMenu.close()
                }
            }
        }
    }

    ToolTip {
        id: tooltip
        text: `${preview.name}\n${preview.tags.join(", ")}${preview.source === "" ? "" : `\n${preview.source}`}`
    }

    Rectangle {
        id: roundMask
        visible: false
        radius: 10
        anchors.fill: parent
        color: "black"

        layer.enabled: true
    }

    Item {
        anchors.fill: parent

        layer.enabled: true
        layer.effect: MultiEffect {
            autoPaddingEnabled: false
            blurEnabled: preview.isBad
            blur: preview.isBad && !preview.isHovered ? 1 : 0
            blurMax: 64
            blurMultiplier: 0.2
            maskEnabled: true
            maskThresholdMin: 0.5
            maskSpreadAtMin: 1.0
            maskSource: roundMask

            Behavior on blur {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 240
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onPressed: {
                preview.isPressed = true
            }

            onReleased: {
                preview.isPressed = false
                Wallpapers.applyWallpaperAsync(preview.wid)
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

        AnimatedImage {
            anchors.fill: parent
            scale: preview.isHovered && !preview.isPressed ? 1.1 : 1
            source: `file://${rootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.webp`
            fillMode: Image.PreserveAspectCrop
            asynchronous: true

            Behavior on scale {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 120
                }
            }
        }
    }
}
