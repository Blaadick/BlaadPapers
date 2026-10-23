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

    ButtonGroup {
        id: contrast
    }

    ButtonGroup {
        id: motion
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

            onClicked: Config.colorScheme = "system"
        }

        Button {
            icon.name: "dark-theme"
            checkable: true
            checked: Config.colorScheme === "dark"
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.colorScheme = "dark"
        }

        Button {
            icon.name: "light-theme"
            checkable: true
            checked: Config.colorScheme === "light"
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.colorScheme = "light"
        }
    }

    RowLayout {
        Label {
            text: qsTr("Motion")
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            icon.name: "system-default"
            checkable: true
            checked: Config.motion === "system"
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.motion = "system"
        }

        Button {
            icon.name: "normal-motion"
            checkable: true
            checked: Config.motion === "normal"
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.motion = "normal"
        }

        Button {
            icon.name: "reduced-motion"
            checkable: true
            checked: Config.motion === "reduced"
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.motion = "reduced"
        }
    }

    RowLayout {
        Label {
            text: qsTr("Contrast")
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            icon.name: "system-default"
            checkable: true
            checked: Config.contrast === "system"
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.contrast = "system"
        }

        Button {
            icon.name: "normal-contrast"
            checkable: true
            checked: Config.contrast === "normal"
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.contrast = "normal"
        }

        Button {
            icon.name: "high-contrast"
            checkable: true
            checked: Config.contrast === "high"
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.contrast = "high"
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
}
