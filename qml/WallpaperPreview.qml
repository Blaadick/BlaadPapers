import QtCore
import QtQuick
import QtQuick.Controls

Item {
    id: preview
    scale: isPressed || !isHovered ? 1 : 1.06

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.OutQuad
            duration: 60
        }
    }

    property string wid
    property string name
    property var tags
    property bool isAnimated
    property bool isBad
    property bool isPressed
    property bool isHovered

    Image {
        anchors.fill: parent
        visible: censor.opacity < 1
        source: `${StandardPaths.writableLocation(StandardPaths.CacheLocation)}/preview/${Screen.width * Screen.devicePixelRatio}x${Screen.height * Screen.devicePixelRatio}/${preview.wid}.webp`
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
    }

    Rectangle {
        id: censor

        anchors.fill: parent
        opacity: preview.isBad && !preview.isHovered ? 1 : 0
        color: "black"

        Text {
            anchors.centerIn: parent
            text: "BAD"
            font.pixelSize: parent.width * 0.5
            font.bold: true
            color: "darkred"
        }

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.OutQuad
                duration: 300
            }
        }
    }

    ToolTip {
        id: tooltip
        text: `${preview.name} (${preview.wid})\n${preview.tags.join(", ")}`
    }

    Menu {
        id: contextMenu

        Action {
            text: "Apply"
            onTriggered: WallpapersModel.applyWallpaper(preview.wid)
        }

        Action {text: "Apply"; icon.name: "settings-configure"}
        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}

        Menu {
            title: "Menu1"

            Action {text: "Apply"; checkable: true}
            Action {text: "Apply"; icon.name: "settings-configure"}
            MenuSeparator {}
            Action {text: "Apply"}

            Menu {
                title: "Menu2"

                Action {text: "Apply"; checkable: true}
                Action {text: "Apply"}
                Action {text: "Apply"}
                Action {text: "Apply"}
                Action {text: "Apply"}
                Action {text: "Apply"}
            }
        }

        MenuSeparator {}

        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}
        Action {text: "Apply"}

        // Action {
        //     text: "Delete"
        //     onTriggered: WallpapersModel.deleteWallpaper(preview.wid)
        // }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onPressed: (event) => {
            if(event.button === Qt.LeftButton) {
                preview.isPressed = true
            } else {
                contextMenu.popup()
            }
        }

        onReleased: (event) => {
            if(event.button === Qt.LeftButton) {
                preview.isPressed = false
                WallpapersModel.applyWallpaper(preview.wid)
            }
        }

        onCanceled: {
            preview.isPressed = false
        }

        onEntered: {
            preview.isHovered = true
            tooltip.visible = true
        }

        onExited: {
            preview.isHovered = false
            tooltip.visible = false
        }
    }
}
