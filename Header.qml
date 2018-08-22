import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    width: parent.width
    height: dp(50)
    color: header_color

    property string header_name

    Button {
        id: column_menu_button
        onClicked: {
            drawer.open()
        }
        height: parent.height
        width: parent.height * 1.2

        background: Rectangle {
            anchors.fill: parent
            color: column_menu_button.pressed ? header_pressed_color : header_color

            Column {
                width: parent.width * 0.6
                height: parent.height * 0.6
                anchors.centerIn: parent

                Item {
                    height: parent.height / 3
                    width: parent.width

                    Rectangle {
                        height: parent.height / 3
                        width: parent.width * 0.8
                        radius: dp(2)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }

                Item {
                    height: parent.height / 3
                    width: parent.width


                    Rectangle {
                        height: parent.height / 3
                        width: parent.width * 0.8
                        radius: dp(2)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }

                Item {
                    height: parent.height / 3
                    width: parent.width


                    Rectangle {
                        height: parent.height / 3
                        width: parent.width * 0.8
                        radius: dp(2)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: parent.header_name
        font.family: "sans-serif"
        font.pixelSize: dp(18)
        color: header_text_color
    }
}

