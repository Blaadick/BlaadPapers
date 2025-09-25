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
            model: WallpapersModel
            delegate: WallpaperPreview {
                width: flow.itemWidth
                height: flow.itemHeight
                wid: wallpaperId
                name: wallpaperName
                tags: wallpaperTags
                isAnimated: isWallpaperAnimated
                isBad: isWallpaperBad
            }
        }
    }
}
