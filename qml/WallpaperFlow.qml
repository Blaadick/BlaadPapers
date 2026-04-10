// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls

Flickable {
    contentHeight: flow.implicitHeight
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    property alias model: flow.model

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }

    Flow {
        id: flow
        width: parent.width
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
                    resolution: wallpaperResolution
                    source: wallpaperSource
                    tags: wallpaperTags
                    isBad: isWallpaperBad
                }
            }
        }
    }
}
