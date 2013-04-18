// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    width: 493
    height: 326
    color: "transparent"

    Flipable {
        id: flipable1
        anchors.fill: parent
        front: Rectangle {
            id: rectangle2
            color: "transparent"
            anchors.fill: parent

            Image {
                id: image1
                anchors.fill: parent
                fillMode: Image.Tile
                source: "qrc:/Courier.Cut/installWindowBack.png"
            }
            Flickable {
                width: 493
                height: 93
                anchors.bottomMargin: 63
                anchors.topMargin: 170
                visible: true
                anchors.fill: parent
                Text {
                    id: text1
                    x: 28
                    y: 4
                    text: qsTr("当您按下“启动Courier”后，程序将：")
                    styleColor: "#ffffff"
                    style: Text.Raised
                    font.bold: true
                    font.family: "微软雅黑"
                    font.pixelSize: 12
                }

                Text {
                    id: text2
                    x: 52
                    y: 25
                    text: "~将Courier程序文件安装至Program Files文件夹\n~设定Courier后台服务程序开机自动启动\n~启动Courier服务程序"
                    styleColor: "#ffffff"
                    style: Text.Raised
                    font.family: "微软雅黑"
                    font.pixelSize: 12
                    Component.onCompleted: {
                        lineHeightMode = Text.FixedHeight
                        lineHeight = 20
                    }
                }
            }
        }
        CheckBox {
            id: agreementCheckbox
            x: 31
            y: 271
            width: 20
            height: 20
            anchors.leftMargin: 31
            anchors.topMargin: 271
            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Text {
            x: 51
            y: 273
            text: "我已阅读并同意Courier相关<a href='#'><font color='red'><u>服务协议</u></font></a>"
            styleColor: "#dbdbdb"
            style: Text.Raised
            font.pixelSize: 12
            font.family: "微软雅黑"

        }
        Rectangle {
            x: 352
            y: 264
            width: launchButton.width
            height: launchButton.height

            StyleButton {
                id: launchButton
                text: "启动Courier"
                z: 1
                onClicked: {
                    dialog.show("dasdsa");
                }
            }

            ShadowRectangle {
                anchors.centerIn: launchButton
                width: launchButton.width
                height: launchButton.height
                color: "transparent"
                opacity: 0.3
            }
        }
    }

    MouseArea {
        id: mouse_region
        anchors.fill: parent
        property variant clickPos: "1,1"
        width: 493
        height: 170
        anchors.bottomMargin: 156
        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            mainWindow.pos = Qt.point(mainWindow.pos.x + delta.x,
                                      mainWindow.pos.y + delta.y)
        }
    }
}
