// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

ApplicationWindow {
    id: mainWindow
    minimumWidth: 280 + 10 * 2
    minimumHeight: searchBar.height + 157 + statusBar.height + 10 * 4
    width: Math.min(minimumWidth * 4, Screen.width)
    height: Math.min(minimumHeight * 3, Screen.height)
    visible: true

    Shortcut {
        sequence: "F5"

        onActivated: {
            Wallpapers.refreshWallpapers()
        }
    }

    DownloadWindow {
        id: downloadWindow
        transientParent: mainWindow
    }

    Menu {
        id: contextMenu

        property bool suppressReopen: false

        onAboutToHide: {
            if(menuButton.hovered) {
                suppressReopen = true
            }
        }

        Menu {
            title: "Add"
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/add.svg"

            Action {
                text: "Install wallpaper(s)"
                icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/install.svg"

                onTriggered: Wallpapers.installWallpapersFromDialog()
            }

            Action {
                text: "Download wallpaper(s)"
                icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/download.svg"

                onTriggered: downloadWindow.show()
            }
        }

        Action {
            text: "Open Config"
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/file_open.svg"

            onTriggered: Qt.openUrlExternally(`file://${Config.configFilePath}`)
        }

        Action {
            text: "Status Bar"
            checkable: true
            checked: Config.statusBarVisible

            onTriggered: Config.statusBarVisible = !Config.statusBarVisible
        }

        Action {
            text: "Bad Tagged Wallpapers"
            checkable: true
            checked: Config.badTaggedWallpapersVisible

            onTriggered: {
                Config.badTaggedWallpapersVisible = !Config.badTaggedWallpapersVisible
                proxy.invalidate()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            spacing: 10

            Button {
                id: menuButton
                icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/menu.svg"
                Layout.preferredHeight: searchBar.height
                Layout.preferredWidth: searchBar.height

                onClicked: {
                    if(contextMenu.suppressReopen) {
                        contextMenu.suppressReopen = false
                    } else {
                        const pos = menuButton.mapToGlobal(0, menuButton.height + 10)
                        contextMenu.popup(pos)
                    }
                }
            }

            TextField {
                id: searchBar
                placeholderText: "Search"
                Layout.fillWidth: true

                onTextChanged: proxy.invalidate()
            }
        }

        SortFilterProxyModel {
            id: proxy
            sourceModel: Wallpapers

            filters: [
                FunctionFilter {
                    function filter(isWallpaperBad: bool): bool {
                        return Config.badTaggedWallpapersVisible || !isWallpaperBad;
                    }
                },
                FunctionFilter {
                    function filter(wallpaperName: string, wallpaperTags: list<string>): bool {
                        if(!searchBar.text) {
                            return true
                        }

                        const filterText = searchBar.text.toLowerCase()
                        return wallpaperName.toLowerCase().includes(filterText)
                            || wallpaperTags.some(tag => tag.toLowerCase().includes(filterText))
                    }
                }
            ]
        }

        WallpaperFlow {
            id: wallpaperFlow
            model: proxy
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            visible: Config.statusBarVisible
        }
    }

    DropArea {
        id: dropArea
        anchors.fill: parent

        onDropped: function(drop) {
            Wallpapers.installWallpapersAsync(drop.urls.map(url => url.toString().substring(7)))
        }
    }

    Rectangle {
        id: dropAreaVisualization
        anchors.fill: parent
        visible: dropArea.containsDrag
        color: "#ffffff"
        opacity: 0.2
    }
}
