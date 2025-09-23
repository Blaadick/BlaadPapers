import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import BlaadPapers

ApplicationWindow {
    visible: true
    minimumWidth: 280 + flow.spacing * 2
    minimumHeight: 157 + flow.spacing * 3 + search.height
    font.family: "monospace"

    menuBar: MenuBar {
        Menu {
            title: "&File"
            Action {
                text: "Open Config"
                onTriggered: Qt.openUrlExternally(`file://${ConfigModel.getConfigPath()}`)
            }
        }

        Menu {
            title: "&Window"
            Action {
                text: "Status Bar"
                checkable: true
                checked: statusBar.visible
                onTriggered: statusBar.visible = !statusBar.visible
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        TextField {
            id: search
            placeholderText: "Search"
            Layout.fillWidth: true
        }

        Item {
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            Flickable {
                contentHeight: flow.implicitHeight
                boundsBehavior: Flickable.StopAtBounds
                anchors {
                    fill: parent
                    // leftMargin: -10
                    // rightMargin: -10
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
                        model: WallpapersModel
                        delegate: WallpaperPreview {
                            width: flow.itemWidth
                            height: flow.itemHeight
                            name: wallpaperName
                            tags: wallpaperTags
                            isBad: isWallpaperBad
                        }
                    }
                }
            }
        }

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
        }
    }
}
