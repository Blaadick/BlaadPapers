// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

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
            checked: true
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/dark-theme.svg"
            checkable: true
            checked: false
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/light-theme.svg"
            checkable: true
            checked: false
            ButtonGroup.group: colorScheme
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }
    }

    RowLayout {
        Label {
            text: qsTr("Reduced Motion")
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/system-default.svg"
            checkable: true
            checked: true
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/normal-motion.svg"
            checkable: true
            checked: false
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/reduced-motion.svg"
            checkable: true
            checked: false
            ButtonGroup.group: motion
            Layout.preferredHeight: height
            Layout.preferredWidth: height
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
            checked: true
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/normal-contrast.svg"
            checkable: true
            checked: false
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height
        }

        Button {
            icon.source: "qrc:/qt/qml/BlaadPapers/resource/icon/high-contrast.svg"
            checkable: true
            checked: false
            ButtonGroup.group: contrast
            Layout.preferredHeight: height
            Layout.preferredWidth: height
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

    Item {
        Layout.fillHeight: true
    }
}
