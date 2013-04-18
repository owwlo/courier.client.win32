// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    // Mirroring function can be used if necessary
    //LayoutMirroring.enabled: true
    //LayoutMirroring.childrenInherit: true

    id: dialogItem
    property bool isHost: true

    color: "transparent"
    state: isHost ? "STATE_HOST" : "STATE_CLIENT"

    states: [
        State {
            name: "STATE_HOST"
            PropertyChanges {
                target: group_A
                anchors.rightMargin: 0
                anchors.leftMargin: 2
            }
            AnchorChanges {
                target: group_A
                anchors.right: parent.right
                anchors.left: group_B.right
            }
            PropertyChanges {
                target: border_image2
                source: "qrc:/Courier.Cut/dialogFrame.png"
                border.right: 6
                border.left: 15
            }
            PropertyChanges {
                target: text_edit1
                anchors.rightMargin: 6
                anchors.leftMargin: 14
            }
            PropertyChanges {
                target: group_B
                anchors.leftMargin: 0
            }
            AnchorChanges {
                target: group_B
                anchors.left: parent.left
                anchors.right: undefined
            }
        },
        State {
            name: "STATE_CLIENT"
            PropertyChanges {
                target: group_A
                anchors.rightMargin: 2
                anchors.leftMargin: 0
            }
            AnchorChanges {
                target: group_A
                anchors.right: group_B.left
                anchors.left: parent.left
            }
            PropertyChanges {
                target: border_image2
                source: "qrc:/Courier.Cut/dialogFrame_R.png"
                border.right: 15
                border.left: 6
            }
            PropertyChanges {
                target: text_edit1
                anchors.rightMargin: 14
                anchors.leftMargin: 6
            }
            PropertyChanges {
                target: group_B
                anchors.rightMargin: 0
            }
            AnchorChanges {
                target: group_B
                anchors.right: parent.right
                anchors.left: undefined
            }
        }
    ]

    Row {
        id: row1
        anchors.fill: parent

        Rectangle {
            id: group_A
            color: "transparent"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            BorderImage {
                id: border_image2
                anchors.fill: parent
                border.bottom: 6
                border.top: 31
            }

            TextEdit {
                id: text_edit1
                color: "#8e8e8e"
                readOnly: true
                text: qsTr("${TEXT_MESSAGE_A}")
                font.bold: true
                font.family: "微软雅黑"
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                z: 1
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: group_B
            width: 51
            color: "transparent"
            radius: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            BorderImage {
                id: border_image1
                z: 1
                anchors.fill: parent
                source: "qrc:/Courier.Cut/picFrame.png"
            }

            Image {
                id: image1
                anchors.fill: parent
                source: "qrc:/Courier.Cut/demoPic.png"
            }
        }
    }
}
