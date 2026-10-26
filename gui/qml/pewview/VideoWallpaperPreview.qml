// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

WallpaperPreview {
    id: control

    required property size wallpaperResolution
    required property int wallpaperFrameRate

    ToolTip {
        id: tooltip
        text: `${control.wallpaperName} [${wallpaperResolution.width}x${wallpaperResolution.height} ${wallpaperFrameRate}fps]\n${control.wallpaperTags.join(", ")}${control.wallpaperSource === "" ? "" : `\n${control.wallpaperSource}`}`
        visible: control.hovered
    }

    background: AnimatedImage {
        anchors.fill: parent
        scale: control.previewScale
        source: control.previewUrl
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: false
    }
}
