// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Button {
    id: control
    clip: true

    required property string wallpaperId
    required property string wallpaperName
    required property url wallpaperRootDir
    required property string wallpaperSource
    required property var wallpaperTags
    required property bool isWallpaperBad
    property string additionalData
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

    Rectangle {
        id: infoBar
        width: control.width
        height: infoColumn.implicitHeight + 8
        y: control.hovered ? control.height - height : control.height + 10
        color: Qt.alpha(control.palette.toolTipBase, 0.8)

        Behavior on y {
            NumberAnimation {
                easing.type: Easing.OutQuad
                duration: control.isMotionReduced ? 0 : 120
            }
        }

        Item {
            id: cornerMask
            anchors.fill: cornerFill
            visible: false
            clip: true
            layer.enabled: true

            Rectangle {
                y: -height / 2
                width: parent.width
                height: parent.height * 2
                radius: 10
                color: "black"
            }
        }

        Rectangle {
            id: cornerFill
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.top
            height: 10
            color: infoBar.color

            layer.enabled: true
            layer.effect: MultiEffect {
                autoPaddingEnabled: false
                maskEnabled: true
                maskInverted: true
                maskThresholdMin: 0.5
                maskSpreadAtMin: 1.0
                maskSource: cornerMask
            }
        }

        Column {
            id: infoColumn
            anchors.centerIn: parent
            width: parent.width - 16

            Label {
                width: parent.width
                font.pointSize: Qt.application.font.pointSize * 0.8
                text: control.additionalData
            }

            Label {
                width: parent.width
                elide: Text.ElideRight
                font.pointSize: Qt.application.font.pointSize * 0.8
                text: control.wallpaperName
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
