// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SettingsTab {
    title: qsTr("General")

    RowLayout {
        Label {
            text: qsTr("Wallpapers Path")
        }

        Item {
            Layout.fillWidth: true
        }

        TextField {
            text: Config.wallpapersPath
            placeholderText: "/home/user/..."
            Layout.preferredWidth: 300

            onEditingFinished: Config.wallpapersPath = text
        }
    }

    RowLayout {
        Label {
            text: qsTr("Default Tags")
            topPadding: defaultTags.topPadding
            Layout.alignment: Qt.AlignTop
        }

        Item {
            Layout.fillWidth: true
        }

        TextArea {
            id: defaultTags
            text: Config.defaultTags.join('\n')
            placeholderText: "Tag1\nTag2\nTag3\n..."
            Layout.preferredWidth: 300

            onEditingFinished: Config.defaultTags = text.split('\n')
        }
    }

    RowLayout {
        Label {
            text: qsTr("Bad Tags")
            topPadding: badTags.topPadding
            Layout.alignment: Qt.AlignTop
        }

        Item {
            Layout.fillWidth: true
        }

        TextArea {
            id: badTags
            text: Config.badTags.join('\n')
            placeholderText: "Tag1\nTag2\nTag3\n..."
            Layout.preferredWidth: 300

            onEditingFinished: Config.badTags = text.split('\n')
        }
    }

    GroupBox {
        title: qsTr("Scripting")
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            ColumnLayout {
                Layout.fillWidth: true

                Label {
                    text: qsTr("Post Install")
                }

                TextField {
                    text: Config.postInstallScript
                    placeholderText: 'notify-send -a BlaadPapers -u low "Wallpaper $WALLPAPER_NAME installed"'
                    Layout.fillWidth: true

                    onEditingFinished: Config.postInstallScript = text
                }
            }

            ColumnLayout {
                Layout.fillWidth: true

                Label {
                    text: qsTr("Post Apply")
                }

                TextField {
                    text: Config.postApplyScript
                    placeholderText: 'notify-send -a BlaadPapers -u low "Wallpaper $WALLPAPER_NAME applied"'
                    Layout.fillWidth: true

                    onEditingFinished: Config.postApplyScript = text
                }
            }

            ColumnLayout {
                Layout.fillWidth: true

                Label {
                    text: qsTr("Post Delete")
                }

                TextField {
                    text: Config.postDeleteScript
                    placeholderText: 'notify-send -a BlaadPapers -u low "Wallpaper $WALLPAPER_NAME deleted"'
                    Layout.fillWidth: true

                    onEditingFinished: Config.postDeleteScript = text
                }
            }
        }
    }
}
