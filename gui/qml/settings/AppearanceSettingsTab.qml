// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SettingsTab {
    title: qsTr("Appearance")

    ButtonGroup {
        id: colorScheme
    }

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
        Label {
            text: qsTr("Native Text Rendering")
        }

        Item {
            Layout.fillWidth: true
        }

        Switch {
            checked: Config.nativeTextRendering

            onToggled: Config.nativeTextRendering = !Config.nativeTextRendering
        }
    }
}
