import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.1

ApplicationWindow {
    visible: true
    width: 480
    height: 640
    title: qsTr("Tabs")

    property color header_color: "#303030"
    property color header_pressed_color: "#c6c6c6"
    property color header_text_color: "white"

    function dp(x){
        if ((Screen.pixelDensity * 25.4) < 120) {
            return x;
        } else {
            return x*((Screen.pixelDensity * 25.4)/160);
        }
    }

    header: Header {
        header_name: "template"
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
//        currentIndex: 0
    }

//    footer: TabBar {
//        id: tabBar
//        currentIndex: swipeView.currentIndex

//        TabButton {
//            text: qsTr("Page 1")
//        }
//        TabButton {
//            text: qsTr("Page 2")
//        }
//    }
}
