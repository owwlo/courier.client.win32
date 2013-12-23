import QtQuick 1.1

Rectangle {
    width: 360
    height: 360
    //color:"red"
    Component.onCompleted: {
        text1.text="adsadasdaadasdasadsasdadsadasdaadasdasadsasdadsadasdaadasdasadsasdadsadasdaadasdasadsasdadsadasdaadasdasadsasd"
    }

    Rectangle {
        width: 300
        color:"red"
        height: text1.height+10
        Text {
            id: text1
            anchors.centerIn: parent
            width: 200
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            text: "Hello Worasdadsadadsadadasdasdasdasdsadld"
            Rectangle {
                z: 1
                color:"#8e8e8e8e"
                anchors.fill: parent
            }
        }
    }
}

