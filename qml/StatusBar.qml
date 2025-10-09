// Copyright (C) 2025 Blaadick
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: layout.width
    height: layout.height

    RowLayout {
        id: layout
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Label {
            id: info
            text: `${Status.statusText}${Status.repeatCount > 1 ? ` x${Status.repeatCount}` : ''}`
        }
    }
}
