import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import BlaadPapers

ApplicationWindow {
    id: mainWindow
    minimumWidth: 280 + 10 * 2
    minimumHeight: (menuBar.visible ? menuBar.height : 0)
        + (searchBar.visible ? searchBar.height : 0)
        + 157
        + (statusBar.visible ? statusBar.height : 0)
        + 10 * 4
    font.family: "monospace"
    visible: true
    menuBar: MenuBar {
        // visible: false

        Menu {
            title: "&File"

            Action {
                text: "Open Config"
                onTriggered: Qt.openUrlExternally(`file://${ConfigModel.getConfigPath()}`)
            }
        }

        Menu {
            title: "&Window"

            Action {
                text: "Status Bar"
                checkable: true
                checked: statusBar.visible
                onTriggered: statusBar.visible = !statusBar.visible
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Button {
            }
            ToolButton {
                icon.name: "document-open"

                onClicked: {
                    // console.log(Qt.quickControls2AvailableStyles())
                }
            }

            TextField {
                id: searchBar
                placeholderText: "Search"
                Layout.fillWidth: true
            }
        }

        WallpaperFlow {
            id: wallpaperFlow
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        StatusBar {
            id: statusBar
            visible: false
            Layout.fillWidth: true
        }
    }
}
