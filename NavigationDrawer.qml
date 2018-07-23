import QtQuick 2.0
import QtQuick.Controls 2.2

Drawer {
    width: parent.width * 0.75
    height: parent.height
    interactive: swipe_view.visible == false ? false : true

    Label {
        text: "Content goes here!"
        anchors.centerIn: parent
    }
}
