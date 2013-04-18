// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    id: rectangle1
    width: parent.width
    height: 30
    color: "transparent"

    Text {
        id: title
        height: 18
        text: "同${MESSAGE_SENDER}(${PHONE_NUMBER})的会话"
        anchors.leftMargin: 5
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        styleColor: "#c8ffffff"
        style: Text.Raised
        font.bold: true
        font.pixelSize: 12
        font.family: "微软雅黑"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    Image {
        id: image1
        width: 40
        height: 14
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/Courier.Cut/dismissButton.png"
    }

}
