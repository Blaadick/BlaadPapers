// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: downloadWindow
    width: 500
    height: 300
    minimumWidth: 400
    minimumHeight: 200
    modality: Qt.NonModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: qsTr("Download Wallpapers")

    Shortcut {
        sequences: [StandardKey.Cancel]
        onActivated: downloadWindow.close()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        TextArea {
            id: input
            placeholderText: "Ein\nDos\nTrios\nNe\nFem\nLiu\n..."
            wrapMode: TextArea.Wrap
            focus: true
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Cancel")

                onClicked: {
                    input.clear()
                    downloadWindow.close()
                }
            }

            Button {
                text: qsTr("Download && Install")
                highlighted: true

                onClicked: {
                    const stringList = input.text
                        .split(/\r?\n/)
                        .map(line => line.trim())
                        .filter(line => line.length > 0)

                    Wallpapers.downloadAndInstallWallpapersAsync(stringList)

                    input.clear()
                    downloadWindow.close()
                }
            }
        }
    }
}
