#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T13:33:59
#
#-------------------------------------------------

QT += widgets
QT += quickwidgets
QT += core gui qml quick location xml network
QT += core gui quick widgets quickwidgets

TARGET = KmlImageTest
TEMPLATE = app

INCLUDEPATH += ../..

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += \
    mainwindow.ui

CONFIG += mobility

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

DISTFILES += \
    test.qml

RESOURCES += \
    kmlimage.qrc

macx{
#QMAKE_MAC_SDK = macosx10.11
}

