// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

WallpaperPreview {
    id: control

    required property size wallpaperResolution

    ToolTip {
        id: tooltip
        text: `${control.wallpaperName} [${wallpaperResolution.width}x${wallpaperResolution.height}]\n${control.wallpaperTags.join(", ")}${control.wallpaperSource === "" ? "" : `\n${control.wallpaperSource}`}`
        visible: control.hovered
    }

    background: Image {
        anchors.fill: parent
        scale: control.previewScale
        source: `${wallpaperRootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.webp`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: false
    }
}
