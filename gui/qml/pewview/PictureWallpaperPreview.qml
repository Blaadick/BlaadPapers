// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

WallpaperPreview {
    id: control

    required property size wallpaperResolution
    additionalData: `${wallpaperResolution.width}x${wallpaperResolution.height}`

    background: Image {
        anchors.fill: parent
        scale: control.previewScale
        source: `${wallpaperRootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.webp`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: false
    }
}
