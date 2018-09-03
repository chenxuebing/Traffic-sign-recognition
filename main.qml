import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.1

ApplicationWindow {
    id: app
    visible: true
    width: 640
    height: 480
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

    onClosing: {
        if (drawer.opened) {
            drawer.close()
            close.accepted = false
        }
        else if (swipe_view.visible == false)
        {
            header.back_button_alias.clicked()
            close.accepted = false
        }
    }

    header: Header {
        id: header
        header_name: ""
    }

    NavigationDrawer {
        id: drawer
    }

    Swipe {
        id: swipe_view
    }
}
