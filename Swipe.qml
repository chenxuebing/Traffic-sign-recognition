import QtQuick 2.0
import QtQuick.Controls 2.2
import QtMultimedia 5.8

import roadVideoFilter 1.0

SwipeView {
    anchors.fill: parent

    Rectangle {
        id: firstPage
        color: "black"

        Camera {
            id: camera

            imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

            imageCapture.resolution: "1280x720"

            exposure {
                exposureCompensation: -1.0
                exposureMode: Camera.ExposurePortrait
            }

            flash.mode: Camera.FlashRedEyeReduction

            imageCapture {
                onImageCaptured: {
                    photoPreview.source = preview
                }
            }
        }

        VideoOutput {
            source: camera
            anchors.fill: parent
            focus : visible
            filters: videoFilter
        }

        RoadVideoFilter {
            id: videoFilter

            onNewSign: {
                console.log(id, name)

                header.header_name = name

                if (fpListModel.count == 6)
                {
                    fpListModel.remove(5)
                }

                fpListModel.append({id : id})
            }

            onRoadStatus: {
                console.log(status)
            }
        }

        Image {
            id: photoPreview
        }

        Rectangle {
            id: fpListBackground

            height: parent.height
            width: parent.height / 6
            color: header_color
            opacity: 0.5
            anchors.right: parent.right
        }

        ListModel {
            id: fpListModel
        }

        Component {
            id: fpListDelegate

            Item {
                width: fpListBackground.width
                height: fpListBackground.width

                Image {
                    anchors.centerIn: parent
                    source: "database/" + id + "_icon.png"
                    width: parent.width - dp(2)
                    height: parent.height - dp(2)
                }
            }
        }

        ListView {
            anchors.fill: fpListBackground
            model: fpListModel
            delegate: fpListDelegate
            interactive: false
        }
    }


    Rectangle {
        id: secondPage
        color: "blue"
    }
}
