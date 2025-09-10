import QtQuick
import QtQuick.Controls
import BlaadPapers

ApplicationWindow {
    font.family: "monospace"
    visible: true

    TextField {
        id: search
        placeholderText: "Search"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
    }

    Item {
        clip: true
        anchors {
            top: search.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 10
            bottomMargin: 10
        }

        Flickable {
            contentHeight: flow.implicitHeight
            boundsBehavior: Flickable.StopAtBounds
            anchors {
                fill: parent
                leftMargin: 10
                rightMargin: 10
            }

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
                    model: WallpaperList
                    delegate: WallpaperPreview {
                        width: flow.itemWidth
                        height: flow.itemHeight
                        name: wallpaperName
                        description: wallpaperDescription
                        isBad: isWallpaperBad
                    }
                }
            }
        }
    }
}
