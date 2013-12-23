import QtQuick 1.1
import Effects 1.0

Rectangle {
    color: "transparent"
    width: 500

    function messageSendRecall(result) {
        if (result === "ok") {
            console.log("QML: send successful, close qml window.")
            window_container.state = "DeathState"

            // 可以安心的去了T_T
            renderArea.isReadyToGo = true;
        }
    }

    function updateMessage(map) {
        var TYPE_MESSAGE_ITEM = 0;
        var TYPE_MESSAGE_PACK = 1;

        console.log("Map:" + map);
        if (map["type"] === TYPE_MESSAGE_ITEM) {
            upperBar.senderName = "";
            upperBar.senderPhone = map["address"];
            messageModel.append({"mIsHost": false, "mContent":map["body"]
                                    , "mUserImageSource":"image://userImage/" + map["userId"]});
        } else if (map["type"] === TYPE_MESSAGE_PACK) {
            upperBar.senderName = "";
            upperBar.senderPhone = map["senderAddress"];
            for(var i in map["messageArray"]) {
                var smallMap = map["messageArray"][i];

                console.log("person:" +  smallMap["person"]);
                console.log("senderId:" +  map["senderId"]);
                messageModel.append({"mIsHost": smallMap["person"] === map["senderId"]
                                        , "mContent":smallMap["body"]
                                        , "mUserImageSource":"image://userImage/" + smallMap["person"]});
            }
        }
    }

    Rectangle {
        id: renderArea
        width: 500
        height: 320
        color: "transparent"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        property bool isReadyToGo: false
        property int listChildrenHeight : 0

        onListChildrenHeightChanged: {
            //console.log(listChildrenHeight);
            //renderArea.height = 82 + (listChildrenHeight > 240 ? 240 : listChildrenHeight);
        }

//        Component.onCompleted: {
//            updateMessage({"address":123132123});
//        }


        //    function getListChildrenHeight() {
        //        for(var obj:listView.children)

        //    }

        Rectangle {
            width: 450
            height: parent.height
            id: window_container
            color: "transparent"
            state: "InvisiableState"
            //anchors.centerIn: renderArea
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top

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
                        onRunningChanged: {
                            if (!running) {
                                console.log("QML: Animation Complete.")
                                Qt.quit()
                            }
                        }
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
                            duration: 750
                            from: 0.6
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
                    when: renderArea.isReadyToGo == true && window_container.opacity == 0
                    StateChangeScript {
                        script: Qt.quit();
                    }
                }
            ]

            ListModel {
                id: messageModel
            }

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
                                              mainWindow.pos.y + delta.y);
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

                UpperBar {
                    id: upperBar
                }

                Rectangle {
                    anchors.top: upperBar.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: inputBox.top
                    anchors.bottomMargin: 3
                    color: "transparent"

                    ListView {
                        id: listView
                        anchors.fill: parent
                        model: messageModel
                        delegate: listDelegate
                        spacing: 5
                        clip: true
                    }
                }

                InputBox {
                    id: inputBox
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5

                    onReplyMessageAccepted: {
                        console.log("Msg Accept from QML.")
                        onQMLReplyMessageAccepted(message);

                        // 可以安心的去了T_T
                        renderArea.isReadyToGo = true;
                    }
                }
            }

            Component {
                id: listDelegate

                Item {
                    id: delegateItem
                    width: listView.width
                    height: dialogItem.height

                    DialogItem {
                        id: dialogItem
                        isHost: mIsHost
                        content: mContent
                        userImageSource: mUserImageSource
                        anchors.right: parent.right
                        anchors.left: parent.left
                    }

                    ListView.onAdd:
                        SequentialAnimation {
                            ScriptAction {
                                script: renderArea.listChildrenHeight += delegateItem.height + listView.spacing;
                            }
                            PropertyAction {
                                target: delegateItem
                                property: "height"
                                value: 0
                            }
                            NumberAnimation {
                                target: delegateItem
                                property: "height"
                                to: delegateItem.height
                                duration: 500
                                easing.type: Easing.InOutQuad
                            }
                            ScriptAction {
                                script: {
                                    listView.positionViewAtEnd();
                                }
                            }
                        }

                    ListView.onRemove: SequentialAnimation {
                        PropertyAction {
                            target: delegateItem
                            property: "ListView.delayRemove"
                            value: true
                        }
                        NumberAnimation {
                            target: delegateItem
                            property: "height"
                            to: 0
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                        PropertyAction {
                            target: delegateItem
                            property: "ListView.delayRemove"
                            value: false
                        }
                    }
                }
            }
        }
    }
}
