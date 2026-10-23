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
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/system-default.svg"
            checkable: true
            checked: Config.colorScheme === "system"
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.colorScheme = "system"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/dark-theme.svg"
            checkable: true
            checked: Config.colorScheme === "dark"
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.colorScheme = "dark"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/light-theme.svg"
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
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/system-default.svg"
            checkable: true
            checked: Config.motion === "system"
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.motion = "system"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/normal-motion.svg"
            checkable: true
            checked: Config.motion === "normal"
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.motion = "normal"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/reduced-motion.svg"
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
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/system-default.svg"
            checkable: true
            checked: Config.contrast === "system"
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.contrast = "system"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/normal-contrast.svg"
            checkable: true
            checked: Config.contrast === "normal"
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height

            onClicked: Config.contrast = "normal"
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/high-contrast.svg"
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
