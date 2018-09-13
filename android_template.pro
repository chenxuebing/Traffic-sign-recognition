QT += quick
QT += core
QT += multimedia
QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        road_video_filter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    database.hpp \
    road_video_filter.h

    PKGCONFIG += link_pkgconfig
    PKGCONFIG += x11
    PKGCONFIG += dlib-1
    PKGCONFIG += opencv

 unix:!android {
    LIBS += -ldlib
    LIBS += -llapack
    LIBS += -lcblas
    LIBS += -lpng
    LIBS += -ljpeg
    LIBS += -pthread
    LIBS += -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv
    LIBS += -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_core
    LIBS += -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking
    LIBS += -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d
    LIBS += -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc
}

android {
    ANDROID_OPENCV = /home/amyrhorod/android_template/android/OpenCV-android-sdk/sdk/native

#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r2.2.0.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r2.3.3.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r3.0.1.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.0.0.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.0.3.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.1.1.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.2.0.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.3.0.so
#    ANDROID_EXTRA_LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.4.0.so

#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r2.2.0.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r2.3.3.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r3.0.1.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.0.0.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.0.3.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.1.1.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.2.0.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.3.0.so
#    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libnative_camera_r4.4.0.so

    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_androidcamera.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_calib3d.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_contrib.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_core.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_features2d.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_flann.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_highgui.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_imgproc.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_info.so
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java.so
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_legacy.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_ml.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_objdetect.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_ocl.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_photo.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_stitching.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_superres.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_ts.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_video.a
    LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_videostab.a

    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libIlmImf.a
    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjasper.a
    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjpeg.a
    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibpng.a
    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibtiff.a
    LIBS += $$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libtbb.a

    INCLUDEPATH += $$ANDROID_OPENCV/jni/include/opencv/
    INCLUDEPATH += $$ANDROID_OPENCV/jni/include/opencv2/
    INCLUDEPATH += $$ANDROID_OPENCV/jni/include/

    ANDROID_DLIB = /home/amyrhorod/android_template/android/Dlib-android-sdk

    ANDROID_EXTRA_LIBS += $$ANDROID_DLIB/libs/armeabi-v7a/libandroid_dlib.so
    LIBS += $$ANDROID_DLIB/libs/armeabi-v7a/libandroid_dlib.so

    INCLUDEPATH += $$ANDROID_DLIB/dlib/
#    INCLUDEPATH += $$ANDROID_DLIB/jni/jni_common/
#    INCLUDEPATH += $$ANDROID_DLIB/jni/jni_detections/
}
