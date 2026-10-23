// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: control

    readonly property bool isMotionReduced: Application.styleHints.accessibility.motionPreference === 1
    property string wid
    property string name
    property string rootDir
    property string source
    property var tags
    property bool isBad
    property bool isPressed
    property bool isHovered

    ContextMenu.menu: Menu {
        id: contextMenu

        Action {
            text: qsTr("Apply")
            icon.name: "apply"

            onTriggered: Wallpapers.applyWallpaperAsync(control.wid)
        }

        Action {
            text: qsTr("Open Folder")
            icon.name: "folder_open"

            onTriggered: Qt.openUrlExternally(`file://${control.rootDir}`)
        }

        Action {
            text: qsTr("Copy Deeplink")
            icon.name: "link"

            onTriggered: Clipboard.copyWallpaperDeeplink(control.wid)
        }

        MenuSeparator {}

        Action {
            text: qsTr("Delete")
            icon.name: "delete"

            onTriggered: Wallpapers.deleteWallpaperAsync(control.wid)
        }
    }

    ToolTip {
        id: tooltip
        text: `${control.name}\n${control.tags.join(", ")}${control.source === "" ? "" : `\n${control.source}`}`
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
            blurEnabled: control.isBad
            blur: control.isBad && !control.isHovered ? 1 : 0
            blurMax: 64
            blurMultiplier: 0.2
            maskEnabled: true
            maskThresholdMin: 0.5
            maskSpreadAtMin: 1.0
            maskSource: roundMask

            Behavior on blur {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: control.isMotionReduced ? 0 : 240
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton
            hoverEnabled: true

            onPressed: {
                control.isPressed = true
            }

            onReleased: {
                control.isPressed = false
                Wallpapers.applyWallpaperAsync(control.wid)
            }

            onCanceled: {
                control.isPressed = false
            }

            onEntered: {
                control.isHovered = true
                tooltip.visible = true
            }

            onExited: {
                control.isHovered = false
                tooltip.visible = false
            }
        }

        AnimatedImage {
            anchors.fill: parent
            scale: !control.isMotionReduced && !control.isPressed && control.isHovered ? 1.1 : 1
            source: `file://${rootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.webp`
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            cache: false

            Behavior on scale {
                NumberAnimation {
                    easing.type: Easing.OutQuad
                    duration: 120
                }
            }
        }
    }
}
