// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Templates as T

T.Button {
    id: control
    clip: true

    required property string wallpaperId
    required property string wallpaperName
    required property url wallpaperRootDir
    required property string wallpaperSource
    required property var wallpaperTags
    required property bool isWallpaperBad
    readonly property bool isMotionReduced: Application.styleHints.accessibility.motionPreference === 1
    property real previewScale: !isMotionReduced && !pressed && hovered ? 1.1 : 1

    Behavior on previewScale {
        NumberAnimation {
            easing.type: Easing.OutQuad
            duration: 120
        }
    }

    Rectangle {
        id: roundMask
        anchors.fill: parent
        visible: false
        radius: 10
        color: "black"
        layer.enabled: true
    }

    layer.enabled: true
    layer.effect: MultiEffect {
        autoPaddingEnabled: false
        blurEnabled: control.isWallpaperBad
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

    ContextMenu.menu: Menu {
        id: contextMenu

        Action {
            text: qsTr("Apply")
            icon.name: "apply"

            onTriggered: Wallpapers.applyWallpaperAsync(control.wallpaperId)
        }

        Action {
            text: qsTr("Open Folder")
            icon.name: "folder-open"

            onTriggered: Qt.openUrlExternally(control.wallpaperRootDir)
        }

        Action {
            text: qsTr("Copy Deeplink")
            icon.name: "link"

            onTriggered: Clipboard.copyWallpaperDeeplink(control.wallpaperId)
        }

        MenuSeparator {}

        Action {
            text: qsTr("Delete")
            icon.name: "edit-delete"

            onTriggered: Wallpapers.deleteWallpaperAsync(control.wallpaperId)
        }
    }

    onClicked: {
        Wallpapers.applyWallpaperAsync(control.wallpaperId)
    }
}
