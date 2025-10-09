// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls

Flickable {
    contentHeight: flow.implicitHeight
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }

    Flow {
        id: flow
        width: parent.width
        spacing: 10

        property int cols: Math.max(Math.floor((width + spacing) / (280 + spacing)), 1)
        property real itemWidth: (width - (cols - 1) * spacing - 1) / cols
        property real itemHeight: itemWidth / (Screen.width / Screen.height)

        Repeater {
            model: Wallpapers
            delegate: Item {
                width: flow.itemWidth
                height: flow.itemHeight
                clip: true

                WallpaperPreview {
                    anchors.fill: parent
                    wid: wallpaperId
                    name: wallpaperName
                    tags: wallpaperTags
                    isAnimated: isWallpaperAnimated
                    isBad: isWallpaperBad
                }
            }
        }
    }
}
