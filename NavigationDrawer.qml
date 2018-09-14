import QtQuick 2.0
import QtQuick.Controls 2.2

Drawer {
    width: app.width > app.height ? parent.width * 0.4 : parent.width * 0.7
    height: parent.height

//    Flickable {
//        anchors.fill: parent
//        contentHeight: content.height
//        contentWidth: content.width

        Column {
            id: content
            width: parent.width

            Button {
//                id: isRoadLineView
                width: parent.width
                height: dp(45)

                background: Rectangle {
                    anchors.fill: parent

                    Text {
                        anchors.centerIn: parent
                        text: "Start game"
                        font.family: "sans-serif"
                        font.pixelSize: dp(18)
                        color: "#303030"
                    }
                }
            }



        }
//    }
}
