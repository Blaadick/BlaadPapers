// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    property alias model: repeater.model

    Rectangle {
        id: roundMask
        visible: false
        radius: 10
        anchors.fill: parent
        color: "black"

        layer.enabled: true
    }

    Flickable {
        id: flick
        anchors.fill: parent
        contentHeight: flow.implicitHeight
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        layer.enabled: true
        layer.effect: MultiEffect {
            maskEnabled: true
            maskThresholdMin: 0.5
            maskSpreadAtMin: 1.0
            maskSource: roundMask
        }

        property real targetContentY: 0

        NumberAnimation on contentY {
            id: scrollAnim
            duration: 240
            easing.type: Easing.OutCubic
        }

        WheelHandler {
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            onWheel: (event) => {
                const step = event.angleDelta.y / 120 * (flow.itemHeight + flow.spacing)
                const maxY = Math.max(0, flick.contentHeight - flick.height)
                flick.targetContentY = Math.min(maxY, Math.max(0, flick.targetContentY - step))

                scrollAnim.stop()
                scrollAnim.from = flick.contentY
                scrollAnim.to = flick.targetContentY
                scrollAnim.start()
            }
        }

        onContentYChanged: {
            if(!scrollAnim.running) {
                targetContentY = contentY
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }

        Flow {
            id: flow
            width: flick.width
            spacing: 10

            property int cols: Math.max(Math.floor((width + spacing) / (280 + spacing)), 1)
            property real itemWidth: (width - (cols - 1) * spacing - 1) / cols
            property real itemHeight: itemWidth / (Screen.width / Screen.height)

            Repeater {
                id: repeater

                delegate: Item {
                    width: flow.itemWidth
                    height: flow.itemHeight
                    clip: true

                    WallpaperPreview {
                        anchors.fill: parent

                        wid: wallpaperId
                        name: wallpaperName
                        rootDir: wallpaperRootDir
                        source: wallpaperSource
                        tags: wallpaperTags
                        isBad: isWallpaperBad
                    }
                }
            }
        }
    }
}
