// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SettingsTab {
    title: qsTr("General")

    RowLayout {
        Label {
            text: qsTr("Wallpapers Folder")
        }

        Item {
            Layout.fillWidth: true
        }

        TextField {
            text: Config.wallpapersFolder
            placeholderText: "/home/user/..."
            Layout.preferredWidth: 300

            onEditingFinished: Config.wallpapersFolder = text
        }
    }

    RowLayout {
        SettingLabel {
            text: qsTr("Default Tags")
            description: qsTr("Tags applied to a wallpaper when it is added")
            topPadding: defaultTags.topPadding
            Layout.alignment: Qt.AlignTop
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
        SettingLabel {
            text: qsTr("Bad Tags")
            description: qsTr("Strict measures are applied to wallpapers with these tags")
            topPadding: badTags.topPadding
            Layout.alignment: Qt.AlignTop
        }

        TextArea {
            id: badTags
            text: Config.badTags.join('\n')
            placeholderText: "Tag1\nTag2\nTag3\n..."
            Layout.preferredWidth: 300

            onEditingFinished: Config.badTags = text.split('\n')
        }
    }

    RowLayout {
        SettingLabel {
            text: qsTr("Delete Wallpapers Permanently")
            description: qsTr("Delete the wallpaper folder immediately without moving it to the trash")
        }

        Switch {
            checked: Config.deleteWallpapersPermanently

            onToggled: Config.deleteWallpapersPermanently = !Config.deleteWallpapersPermanently
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
