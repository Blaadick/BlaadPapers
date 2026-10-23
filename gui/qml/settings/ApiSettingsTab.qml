// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SettingsTab {
    title: qsTr("API")

    GroupBox {
        title: "Wallhaven"
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent

            Label {
                text: qsTr("API Key")
            }

            Item {
                Layout.fillWidth: true
            }

            TextField {
                text: Config.wallhavenApiKey
                placeholderText: "Ab3dE5fG7hJ9kL1mN2pQ4rS6tU8vW0xY"
                echoMode: TextInput.Password
                Layout.preferredWidth: 300

                onEditingFinished: Config.wallhavenApiKey = text
            }
        }
    }

    GroupBox {
        title: "Danbooru"
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            RowLayout {
                Label {
                    text: qsTr("Login")
                }

                Item {
                    Layout.fillWidth: true
                }

                TextField {
                    text: Config.danbooruLogin
                    placeholderText: qsTr("Username")
                    Layout.preferredWidth: 300

                    onEditingFinished: Config.danbooruLogin = text
                }
            }

            RowLayout {
                Label {
                    text: qsTr("API Key")
                }

                Item {
                    Layout.fillWidth: true
                }

                TextField {
                    text: Config.danbooruApiKey
                    placeholderText: "aB3dE5fG7hJ9kL1mN2pQ4rS6"
                    echoMode: TextInput.Password
                    Layout.preferredWidth: 300

                    onEditingFinished: Config.danbooruApiKey = text
                }
            }
        }
    }
}
