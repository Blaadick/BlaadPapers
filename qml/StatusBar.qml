import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    spacing: 10

    Label {
        id: info
        text: StatusModel.statusText
    }

    Item {
        Layout.fillWidth: true
    }

    Label {
        id: wallpaperCount
        text: WallpapersModel.rowCount()
    }
}
