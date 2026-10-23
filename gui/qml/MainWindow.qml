// Copyright (C) 2025-2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    minimumWidth: 280 + 10 * 2
    minimumHeight: searchBar.height + 157 + statusBar.height + 10 * 4
    width: Math.min(minimumWidth * 4, Screen.width)
    height: Math.min(minimumHeight * 3, Screen.height)
    visible: true

    Binding {
        target: Application.styleHints
        property: "colorScheme"
        value: Config.colorScheme === "dark" ? Qt.ColorScheme.Dark : Config.colorScheme === "light" ? Qt.ColorScheme.Light : Qt.ColorScheme.Unknown
    }

    Shortcut {
        sequences: [StandardKey.Refresh, "Ctrl+R"]

        onActivated: {
            Wallpapers.refreshWallpapers()
        }
    }

    FileDialog {
        id: installFileDialog
        title: qsTr("Select Files")
        fileMode: FileDialog.OpenFiles
        options: FileDialog.ReadOnly
        nameFilters: Wallpapers.getFileDialogNameFilters()

        onAccepted: Wallpapers.downloadAndInstallWallpapersAsync(selectedFiles)
    }

    DownloadWindow {
        id: downloadWindow
        transientParent: mainWindow
    }

    SettingsWindow {
        id: settingsWindow
        transientParent: mainWindow

        GeneralSettingsTab {}

        AppearanceSettingsTab {}

        ApiSettingsTab {}
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            spacing: 10

            Button {
                id: mainMenuButton
                icon.name: "application-menu"
                Layout.preferredHeight: searchBar.height
                Layout.preferredWidth: searchBar.height

                onClicked: mainMenu.visible ? mainMenu.close() : mainMenu.open()

                Menu {
                    id: mainMenu
                    y: mainMenuButton.height + 4
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    Menu {
                        title: qsTr("Add")
                        icon.name: "add"

                        Action {
                            text: qsTr("Install Wallpaper(s)")
                            icon.name: "package"

                            onTriggered: installFileDialog.open()
                        }

                        Action {
                            text: qsTr("Download Wallpaper(s)")
                            icon.name: "download"

                            onTriggered: downloadWindow.show()
                        }
                    }

                    Action {
                        text: qsTr("Settings")
                        icon.name: "settings"
                        shortcut: "Ctrl+Alt+S"

                        onTriggered: settingsWindow.show()
                    }

                    Action {
                        text: qsTr("Bad Wallpapers")
                        checkable: true
                        checked: Config.badTaggedWallpapersVisible
                        shortcut: "Ctrl+B"

                        onTriggered: {
                            Config.badTaggedWallpapersVisible = !Config.badTaggedWallpapersVisible
                            proxy.invalidate()
                        }
                    }
                }
            }

            SearchField {
                id: searchBar
                placeholderText: qsTr("Search")
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
        anchors.fill: parent
        keys: ["text/uri-list"]

        onDropped: drop => {
            if(drop.hasHtml) {
                const url = drop.html.match(/\bsrc=["']([^"']+)["']/i)
                if(!url) {
                    drop.accept(Qt.IgnoreAction)
                    return
                }

                Wallpapers.downloadAndInstallWallpaperAsync(url[1])
            } else {
                Wallpapers.downloadAndInstallWallpapersAsync(drop.urls)
            }

            drop.acceptProposedAction()
        }

        Rectangle {
            anchors.fill: parent
            visible: parent.containsDrag
            color: "#ffffff"
            opacity: 0.2

            HoverHandler {
                cursorShape: Qt.DragCopyCursor
            }
        }
    }
}
