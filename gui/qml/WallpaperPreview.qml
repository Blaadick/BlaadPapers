// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Templates as T

T.Button {
    id: control

    property string wid
    property string name
    property url rootDir
    property string source
    property var tags
    property bool isBad
    readonly property bool isMotionReduced: Application.styleHints.accessibility.motionPreference === 1

    Rectangle {
        id: roundMask
        visible: false
        radius: 10
        anchors.fill: parent
        color: "black"

        layer.enabled: true
    }

    layer.enabled: true
    layer.effect: MultiEffect {
        autoPaddingEnabled: false
        blurEnabled: control.isBad
        blur: !control.hovered ? 1 : 0
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

    background: AnimatedImage {
        anchors.fill: parent
        scale: !control.isMotionReduced && !control.pressed && control.hovered ? 1.1 : 1
        source: `${rootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.webp`
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

    ContextMenu.menu: Menu {
        id: contextMenu

        Action {
            text: qsTr("Apply")
            icon.name: "apply"

            onTriggered: Wallpapers.applyWallpaperAsync(control.wid)
        }

        Action {
            text: qsTr("Open Folder")
            icon.name: "folder-open"

            onTriggered: Qt.openUrlExternally(control.rootDir)
        }

        Action {
            text: qsTr("Copy Deeplink")
            icon.name: "link"

            onTriggered: Clipboard.copyWallpaperDeeplink(control.wid)
        }

        MenuSeparator {}

        Action {
            text: qsTr("Delete")
            icon.name: "edit-delete"

            onTriggered: Wallpapers.deleteWallpaperAsync(control.wid)
        }
    }

    ToolTip {
        id: tooltip
        text: `${control.name}\n${control.tags.join(", ")}${control.source === "" ? "" : `\n${control.source}`}`
        visible: control.hovered
    }

    onClicked: {
        Wallpapers.applyWallpaperAsync(control.wid)
    }
}
