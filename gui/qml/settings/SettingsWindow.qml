// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Qt.labs.qmlmodels

ApplicationWindow {
    width: 900
    height: 500
    minimumWidth: layout.implicitWidth + 2 * layout.anchors.margins
    minimumHeight: layout.implicitHeight + 2 * layout.anchors.margins
    modality: Qt.NonModal
    flags: Qt.Tool | Qt.WindowStaysOnTopHint
    title: qsTr("Settings")

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            spacing: 10
            Layout.fillWidth: true
            Layout.fillHeight: true

            TreeView {
                id: tabs
                clip: true
                Layout.fillHeight: true
                Layout.preferredWidth: contentWidth

                model: TreeModel {
                    TableModelColumn {
                        display: "name"
                    }

                    rows: [
                        {
                            name: qsTr("General"),
                            page: 0,
                            rows: [
                                {name: qsTr("API"), page: 2}
                            ]
                        },
                        {
                            name: qsTr("Interface"),
                            page: 1
                        }
                    ]
                }

                selectionModel: ItemSelectionModel {}

                delegate: TreeViewDelegate {}

                ScrollBar.vertical: ScrollBar {}

                Component.onCompleted: {
                    expandRecursively()
                    selectionModel.setCurrentIndex(index(0, 0), ItemSelectionModel.SelectCurrent)
                }
            }

            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true

                StackLayout {
                    anchors.fill: parent
                    currentIndex: tabs.selectionModel.currentIndex.valid ? tabs.model.getRow(tabs.selectionModel.currentIndex).page : 0

                    GeneralSettings {}

                    GuiSettings {}

                    ApiSettings {}
                }
            }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Button {
                text: qsTr("Open Config")
                flat: true

                onClicked: Qt.openUrlExternally(`file://${Config.configFilePath}`)
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
            }
        }
    }
}
