QT += quick
QT += core

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
    traffic_sign_recognition.cpp

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
    android/gradlew.bat \
    database/0_icon.png \
    database/1_icon.png \
    database/2_icon.png \
    database/3_icon.png \
    database/4_icon.png \
    database/5_icon.png \
    database/6_icon.png \
    database/7_icon.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    database.hpp \
    traffic_sign_recognition.h

PKGCONFIG += link_pkgconfig
PKGCONFIG += x11
PKGCONFIG += dlib-1

LIBS += -ldlib
LIBS += -llapack
LIBS += -lcblas
LIBS += -lpng
LIBS += -ljpeg
LIBS += -pthread
