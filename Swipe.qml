import QtQuick 2.0
import QtQuick.Controls 2.2
import QtMultimedia 5.8

SwipeView {
    anchors.fill: parent
//        currentIndex: 0


    Rectangle {
        id: firstPage
//            anchors.fill: parent
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
        }

        Image {
            id: photoPreview
        }

    }


    Rectangle {
        id: secondPage
//            anchors.fill: parent
        color: "blue"
    }
}
