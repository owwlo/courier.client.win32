import QtQuick 1.1

Rectangle {
    id: renderArea
    width: 320
    height: 120
    color: "transparent"
    property bool isReadyToGo: false

    function messageSendRecall(result) {
        if (result === "ok") {
            console.log("QML: send successful, close qml window.")
            window_container.state = "DeathState"
        }
    }

    Rectangle {
        id: window_container
        width: parent.width - 50
        height: parent.height - 50
        color: "transparent"
        state: "InvisiableState"
        anchors.centerIn: renderArea

        Component.onCompleted: {
            window_container.state = "AliveState"
        }

        //    Behavior on opacity {
        //        NumberAnimation {
        //            properties:"opacity"
        //            duration: 1000
        //            easing.type: Easing.OutExpo
        //            onCompleted: {
        //                console.log("QML: Animation Complete.");
        //                Qt.quit();
        //            }
        //        }
        //    }
        transitions: [
            Transition {
                from: "AliveState"
                to: "DeathState"
                ParallelAnimation {
                    NumberAnimation {
                        target: window_container
                        properties: "opacity"
                        duration: 500
                        from: 1
                        to: 0
                        easing.type: Easing.OutExpo
                    }
                    PropertyAnimation {
                        target: window_container
                        properties: "scale"
                        duration: 500
                        from: 1
                        to: 0.8
                        easing.type: Easing.Linear
                    }
                    NumberAnimation {
                        target: windowRotation
                        properties: "angle"
                        duration: 500
                        from: 0
                        to: -40
                        easing.type: Easing.Linear
                    }
                }
            },
            Transition {
                from: "InvisiableState"
                to: "AliveState"
                ParallelAnimation {
                    NumberAnimation {
                        target: window_container
                        properties: "opacity"
                        duration: 500
                        from: 0
                        to: 1
                        easing.type: Easing.OutExpo
                    }
                    NumberAnimation {
                        target: window_container
                        properties: "scale"
                        duration: 500
                        from: 0.8
                        to: 1
                        easing.type: Easing.OutElastic
                    }
                }
            }
        ]

        transform: [
            Rotation {
                id: windowRotation
                origin.x: 225
                origin.y: 0
                axis {
                    x: 1
                    y: 0
                    z: 0
                }
                angle: 0
            }
        ]

        states: [
            State {
                name: "InvisiableState"
                PropertyChanges {
                    target: window_container
                    smooth: true
                    opacity: 0
                }
            },
            State {
                name: "AliveState"
                PropertyChanges {
                    target: window_container
                    smooth: false
                }
            },
            State {
                name: "DeathState"
                PropertyChanges {
                    target: window_container
                    smooth: true
                }
            },
            State {
                name: "ReadyToGoT_T"
                when: isReadyToGo == true && window_container.opacity == 0
                StateChangeScript {
                    script: Qt.quit();
                }
            }
        ]

        BorderImage {
            id: upper_back
            x: 0
            y: 0
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: 0
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            source: "qrc:/Courier.Cut/back.png"
            anchors.fill: parent
            border {
                left: 11
                top: 8
                right: 12
                bottom: 13
            }
            anchors.centerIn: parent
            horizontalTileMode: BorderImage.Stretch
            verticalTileMode: BorderImage.Stretch
        }

        MouseArea {
            id: mouse_region
            anchors.fill: parent
            property variant clickPos: "1,1"
            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
            }
            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                mainWindow.pos = Qt.point(mainWindow.pos.x + delta.x,
                                          mainWindow.pos.y + delta.y)
            }
        }

        Rectangle {
            id: mainGroup
            width: parent.width
            anchors.rightMargin: 15
            anchors.leftMargin: 15
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.fill: parent
            color: "transparent"

            InputBox {
                id: inputBox
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5

                onReplyMessageAccepted: {
                    console.log("Msg Accept from QML.");
                    onAuthenticateCodeAccepted(message);
                    window_container.state = "DeathState";

                    // 可以安心的去了T_T
                    isReadyToGo = true;
                }
            }

            Text {
                id: text1
                text: qsTr(" 输入客户端链接码")
                font.bold: true
                styleColor: "#ffffff"
                style: Text.Raised
                font.family: "微软雅黑"
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 1
                font.pixelSize: 12
            }
        }
    }
}
