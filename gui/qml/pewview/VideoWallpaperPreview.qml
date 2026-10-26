// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtMultimedia

WallpaperPreview {
    id: control

    required property size wallpaperResolution
    required property int wallpaperFrameRate

    ToolTip {
        id: tooltip
        text: `${control.wallpaperName} [${wallpaperResolution.width}x${wallpaperResolution.height} ${wallpaperFrameRate}fps]\n${control.wallpaperTags.join(", ")}${control.wallpaperSource === "" ? "" : `\n${control.wallpaperSource}`}`
        visible: control.hovered
    }

    background: VideoOutput {
        id: videoOutput
        anchors.fill: parent
        scale: control.previewScale
        fillMode: VideoOutput.PreserveAspectCrop

        MediaPlayer {
            source: `${wallpaperRootDir}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}.mp4`
            videoOutput: videoOutput
            loops: MediaPlayer.Infinite
            autoPlay: true
        }
    }
}
