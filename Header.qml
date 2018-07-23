import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    width: parent.width
    height: dp(50)
    color: header_color

    property string header_name
    property var back_button_alias: back_button

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

    Button {
        id: back_button
        onClicked: {
            settings_page.visible = false
            swipe_view.visible = true
            settings_button.visible = true
            column_menu_button.visible = true
            back_button.visible = false
        }
        height: parent.height
        width: parent.height * 1.2
        visible: false

        background: Rectangle {
            anchors.fill: parent
            color: back_button.pressed ? header_pressed_color : header_color

            Column {
                width: parent.width * 0.6
                height: parent.height * 0.55
                anchors.centerIn: parent

                Item {
                    height: parent.height / 2
                    width: parent.width

                    Rectangle {
                        height: parent.height / 4.5
                        width: parent.width * 0.6
                        radius: dp(2)
                        color: header_text_color
                        anchors.centerIn: parent
                        rotation: 135
                    }
                }

                Item {
                    height: parent.height / 2
                    width: parent.width

                    Rectangle {
                        height: parent.height / 4.5
                        width: parent.width * 0.6
                        radius: dp(2)
                        color: header_text_color
                        anchors.centerIn: parent
                        rotation: 45
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

    Button {
        id: settings_button
        onClicked: {
            settings_page.visible = true
            swipe_view.visible = false
            settings_button.visible = false
            column_menu_button.visible = false
            back_button.visible = true
        }
        height: parent.height
        width: parent.height * 1.2
        anchors.right: parent.right

        background: Rectangle {
            anchors.fill: parent
            color: settings_button.pressed ? header_pressed_color : header_color

            Column {
                width: parent.width * 0.6
                height: parent.height * 0.6
                anchors.centerIn: parent

                Item {
                    height: parent.height / 3
                    width: parent.width

                    Rectangle {
                        height: parent.height * 0.5
                        width: parent.height * 0.5
                        radius: dp(4)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }

                Item {
                    height: parent.height / 3
                    width: parent.width


                    Rectangle {
                        height: parent.height * 0.5
                        width: parent.height * 0.5
                        radius: dp(4)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }

                Item {
                    height: parent.height / 3
                    width: parent.width


                    Rectangle {
                        height: parent.height * 0.5
                        width: parent.height * 0.5
                        radius: dp(4)
                        color: header_text_color
                        anchors.centerIn: parent
                    }
                }
            }

        }
    }
}

