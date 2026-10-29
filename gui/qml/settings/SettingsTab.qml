// Copyright (C) 2026 Blaadick
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Flickable {
    id: tab
    implicitWidth: column.implicitWidth + 10
    contentHeight: column.implicitHeight
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    property string title
    property string icon
    default property alias content: column.data

    ScrollBar.vertical: ScrollBar {}

    ColumnLayout {
        id: column
        width: tab.width - 10
        spacing: 10
    }
}
