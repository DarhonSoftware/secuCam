TEMPLATE = app

QT += quick network quickcontrols2

TARGET = scclient

SOURCES += main.cpp \
    dr_tcpclient.cpp \
    globalobject.cpp \
    about.cpp \
    dr_crypt.cpp \
    dr_standarditemmodel.cpp

RESOURCES += \
    scclient.qrc

HEADERS += \
    dr_tcpclient.h \
    globalobject.h \
    about.h \
    dr_crypt.h \
    dr_standarditemmodel.h

DISTFILES += \
    packaging/android/AndroidManifest.xml \
    packaging/android/build.gradle \
    packaging/android/gradle.properties \
    packaging/android/gradle/wrapper/gradle-wrapper.jar \
    packaging/android/gradle/wrapper/gradle-wrapper.properties \
    packaging/android/gradlew \
    packaging/android/gradlew.bat \
    packaging/android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/packaging/android



