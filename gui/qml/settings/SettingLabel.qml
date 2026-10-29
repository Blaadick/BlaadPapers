// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    spacing: 2
    Layout.fillWidth: true

    property alias text: title.text
    property alias description: description.text
    property alias topPadding: title.topPadding

    Label {
        id: title
        Layout.fillWidth: true
    }

    Label {
        id: description
        enabled: false
        wrapMode: Text.Wrap
        font.pointSize: Qt.application.font.pointSize * 0.85
        Layout.fillWidth: true
        Layout.preferredWidth: 0
    }
}
