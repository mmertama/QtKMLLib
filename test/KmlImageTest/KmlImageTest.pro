#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T13:33:59
#
#-------------------------------------------------

include("../config.pri")

QT       += core gui qml quick location xml quickwidgets network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = KmlImageTest
TEMPLATE = app

INCLUDEPATH += ../..

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        linepath.cpp \
        polymesh.cpp

HEADERS  += mainwindow.h \
    linepath.h \
    polymesh.h

HEADERS += \
    ../linedraw/geoline.h \
    ../linedraw/line_p.h \
    ../linedraw/line.h \
    ../linedraw/linecollection.h \
    ../linedraw/lineitem.h

SOURCES += \
    ../linedraw/geoline.cpp \
    ../linedraw/line.cpp \
    ../linedraw/linecollection.cpp \
    ../linedraw/lineitem.cpp

INCLUDEPATH += ../linedraw

FORMS    += \
    mainwindow.ui

CONFIG += mobility

APP_SEARCH_PATH=$${PROJECT_LIBS}
#APP_SEARCH_PATH=$${_PRO_FILE_PWD_}/../../../../../lib
message("Note: non system search path:")
message($$APP_SEARCH_PATH)

LIBS += -L$$APP_SEARCH_PATH -lqtkml -llibkml
unix: LIBS += -lexpat

DISTFILES += \
    test.qml

RESOURCES += \
    kmlimage.qrc

macx{
# QMAKE_MAC_SDK = macosx10.11
LIBS += -L/usr/local/opt/expat/lib -lexpat
}

