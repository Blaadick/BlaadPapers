// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 500
    height: 300
    minimumWidth: 400
    minimumHeight: 200
    modality: Qt.NonModal
    flags: Qt.Dialog
    title: "Download Wallpapers"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        TextArea {
            id: input
            placeholderText: "https://example.com/some_file1.ext\nhttps://example.com/some_file2.ext\n..."
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
                text: "Cancel"

                onClicked: {
                    input.clear()
                    downloadWindow.close()
                }
            }

            Button {
                text: "Download && Install"
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
