// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 10

    RowLayout {
        Label {
            text: "Wallpapers Path"
        }

        Item {
            Layout.fillWidth: true
        }

        TextField {
            Layout.preferredWidth: 300
        }
    }

    Item {
        Layout.fillHeight: true
    }
}
