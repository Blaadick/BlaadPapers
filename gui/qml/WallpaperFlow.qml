// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls

Flickable {
    id: flick
    contentHeight: flow.implicitHeight
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    property alias model: flow.model
    property real targetContentY: 0

    NumberAnimation {
        id: scrollAnim
        target: flick
        property: "contentY"
        duration: 280
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
        policy: ScrollBar.AlwaysOn
    }

    Flow {
        id: flow
        width: flick.width
        spacing: 10

        property alias model: repeater.model
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
                    resolution: wallpaperResolution
                    source: wallpaperSource
                    tags: wallpaperTags
                    isBad: isWallpaperBad
                }
            }
        }
    }
}
