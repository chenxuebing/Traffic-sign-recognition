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

                exposure {
                    exposureCompensation: -1.0
                    exposureMode: Camera.ExposurePortrait
                }

                flash.mode: Camera.FlashRedEyeReduction

                imageCapture {
                    onImageCaptured: {
                        photoPreview.source = preview  // Show the preview in an Image
                    }
                }
            }

        VideoOutput {
            source: camera
            anchors.fill: parent
            focus : visible // to receive focus and capture key events when visible
            filters: videoFilter
        }

        RoadVideoFilter {
            id: videoFilter
            // orientation: videoOutput.orientation
        }

        Image {
            id: photoPreview
        }

    }


    Rectangle {
        id: secondPage
        color: "blue"
    }
}
