// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

ApplicationWindow {
    id: mainWindow
    minimumWidth: 280 + 10 * 2
    minimumHeight: searchBar.height + 157 + (statusBar.visible ? statusBar.height : 0) + 10 * 4
    visible: true

    Menu {
        id: contextMenu

        Action {
            text: "Open Config"
            icon.source: "qrc:/BlaadPapers/resource/icon/edit.svg"
            onTriggered: Qt.openUrlExternally(`file://${Config.configFilePath}`)
        }

        Action {
            text: "Status Bar"
            checkable: true
            checked: statusBar.visible
            onTriggered: Config.statusBarVisible = !statusBar.visible
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            ToolButton {
                icon.source: "qrc:/BlaadPapers/resource/icon/menu.svg"

                onClicked: {
                    const pos = parent.mapToGlobal(0, parent.height + 10)
                    contextMenu.popup(pos)
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
            model: Wallpapers

            filters: [
                FunctionFilter {
                    component WallpaperData: QtObject {
                        property string wallpaperName
                        property var wallpaperTags
                    }

                    function filter(data: WallpaperData): bool {
                        if(!searchBar.text) {
                            return true
                        }

                        const filterText = searchBar.text.toLowerCase()
                        return data.wallpaperName.toLowerCase().includes(filterText) ||
                            data.wallpaperTags.some(tag => tag.toLowerCase().includes(filterText))
                    }
                }
            ]
        }

        WallpaperFlow {
            id: wallpaperFlow
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: proxy
        }

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            visible: Config.statusBarVisible
        }
    }
}
