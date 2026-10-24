// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: settingsWindow
    width: 900
    height: 500
    minimumWidth: layout.implicitWidth + 2 * layout.anchors.margins
    minimumHeight: layout.implicitHeight + 2 * layout.anchors.margins
    modality: Qt.NonModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: qsTr("Settings")

    default property alias content: pages.data

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            spacing: 10
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: tabs
                spacing: 4
                model: pages.children
                Layout.fillHeight: true
                Layout.preferredWidth: 150
                boundsBehavior: Flickable.StopAtBounds

                delegate: ItemDelegate {
                    required property var modelData
                    required property int index

                    width: ListView.view.width
                    text: modelData.title
                    highlighted: ListView.isCurrentItem

                    onClicked: tabs.currentIndex = index
                }

                ScrollBar.vertical: ScrollBar {}
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true

                StackLayout {
                    id: pages
                    anchors.fill: parent
                    currentIndex: tabs.currentIndex
                }
            }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Button {
                text: qsTr("Open Configs")
                flat: true

                onClicked: Qt.openUrlExternally(Config.configDirUrl)
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Reset")
            }

            Button {
                text: qsTr("Ok")
                highlighted: true

                onClicked: settingsWindow.hide()
            }
        }
    }
}
