TEMPLATE = app

QT += qml quick location xml webview

CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += ../..

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

unix{
PROJECT_LIBS = $${PWD}/../../libs/unix
}

macx{
PROJECT_LIBS = $${PWD}/../../libs/osx
}

win32{
PROJECT_LIBS = $${PWD}/../../libs/win
}

android{
    config += ANDROID_NDK_PLATFORM=android-26
    PROJECT_LIBS = $${PWD}/../../libs/android
}

LIBS += -L$$PROJECT_LIBS -lqtkml -llibkml
unix: LIBS += -lexpat

message("Note: non system search path:")
message($$PROJECT_LIBS)


# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    Teknopolis.kml

macx{
#QMAKE_MAC_SDK = macosx10.11
}
