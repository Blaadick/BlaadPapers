// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: mainWindow
    minimumWidth: 280 + 10 * 2
    minimumHeight: searchBar.height + 157 + (statusBar.visible ? statusBar.height : 0) + 10 * 4
    visible: true

    Menu {
        id: contextMenu

        Action {
            text: "Open Config"
            icon.name: "edittext"
            onTriggered: Qt.openUrlExternally(`file://${ConfigModel.getConfigFilePath()}`)
        }

        Action {
            text: "Status Bar"
            checkable: true
            checked: statusBar.visible
            onTriggered: statusBar.visible = !statusBar.visible
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            ToolButton {
                icon.name: "settings-configure"

                onClicked: {
                    contextMenu.popup()
                }
            }

            TextField {
                id: searchBar
                placeholderText: "Search"
                Layout.fillWidth: true
            }
        }

        WallpaperFlow {
            id: wallpaperFlow
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        StatusBar {
            id: statusBar
            Layout.fillWidth: true
            visible: false
        }
    }
}
