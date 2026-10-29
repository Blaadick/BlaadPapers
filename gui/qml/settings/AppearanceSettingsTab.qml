// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SettingsTab {
    title: qsTr("Appearance")
    icon: "preferences-appearance"

    ButtonGroup {
        id: colorScheme
    }

    GroupBox {
        title: qsTr("Theme")
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            RowLayout {
                Label {
                    text: qsTr("Color Scheme")
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    icon.name: "system-default"
                    checkable: true
                    checked: Config.colorScheme === "system"
                    ButtonGroup.group: colorScheme
                    Layout.preferredHeight: height
                    Layout.preferredWidth: height

                    onClicked: {
                        Config.colorScheme = "system"
                        Application.styleHints.colorScheme = Qt.ColorScheme.Unknown
                    }
                }

                Button {
                    icon.name: "dark-theme"
                    checkable: true
                    checked: Config.colorScheme === "dark"
                    ButtonGroup.group: colorScheme
                    Layout.preferredHeight: height
                    Layout.preferredWidth: height

                    onClicked: {
                        Config.colorScheme = "dark"
                        Application.styleHints.colorScheme = Qt.ColorScheme.Dark
                    }
                }

                Button {
                    icon.name: "light-theme"
                    checkable: true
                    checked: Config.colorScheme === "light"
                    ButtonGroup.group: colorScheme
                    Layout.preferredHeight: height
                    Layout.preferredWidth: height

                    onClicked: {
                        Config.colorScheme = "light"
                        Application.styleHints.colorScheme = Qt.ColorScheme.Light
                    }
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Language")
                }

                Item {
                    Layout.fillWidth: true
                }

                ComboBox {
                    textRole: "text"
                    valueRole: "value"
                    implicitContentWidthPolicy: ComboBox.WidestText
                    model: [
                        {value: "system", text: qsTr("System")},
                        {value: "de", text: "Deutsch"},
                        {value: "en_GB", text: "English (UK)"},
                        {value: "en_US", text: "English (US)"},
                        {value: "es", text: "Español"},
                        {value: "fr", text: "Français"},
                        {value: "it", text: "Italiano"},
                        {value: "pl", text: "Polski"},
                        {value: "pt_BR", text: "Português (Brasil)"},
                        {value: "tr", text: "Türkçe"},
                        {value: "be", text: "Беларуская"},
                        {value: "ru", text: "Русский"},
                        {value: "uk", text: "Українська"},
                        {value: "ja", text: "日本語"},
                        {value: "zh_CN", text: "简体中文"},
                        {value: "ko", text: "한국어"}
                    ]

                    Component.onCompleted: currentIndex = indexOfValue(Config.language)
                    onModelChanged: currentIndex = indexOfValue(Config.language)

                    onActivated: {
                        Config.language = currentValue
                        Qt.uiLanguage = Config.uiLanguage
                    }
                }
            }
        }
    }

    GroupBox {
        title: qsTr("Window")
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            RowLayout {
                Label {
                    text: qsTr("Show Status Bar")
                }

                Item {
                    Layout.fillWidth: true
                }

                Switch {
                    checked: Config.statusBarVisible

                    onToggled: Config.statusBarVisible = !Config.statusBarVisible
                }
            }

            RowLayout {
                SettingLabel {
                    text: qsTr("Native Text Rendering")
                    description: qsTr(
                        "Render text with the system font engine instead of the Qt one. Text looks like in other apps but scales worse. Requires restart")
                }

                Switch {
                    checked: Config.nativeTextRendering

                    onToggled: Config.nativeTextRendering = !Config.nativeTextRendering
                }
            }
        }
    }
}
