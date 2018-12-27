#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T10:09:57
#
#-------------------------------------------------

include("third_party/libkml.pri")
include(config.pri)


QT -= gui
QT += quick positioning

TARGET = qtkml
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

#pathsTarget.commands = python $${_PRO_FILE_PWD_}/createlink.py $${_PRO_FILE_PWD_}/paths/uriparser $${_PRO_FILE_PWD_}/libkml-1.2.0/third_party/uriparser-0.7.5
#pathsTarget.depends =
#pathsTarget.target = extra
#QMAKE_EXTRA_TARGETS += pathsTarget
#PRE_TARGETDEPS += extra

#git clone https://android.googlesource.com/platform/external/expat

SOURCES += \
    rendervisitor.cpp \
    graphicsvisitor.cpp \
    stylevisitor.cpp \
    mercatorprojection.cpp \
    kmldocument.cpp \
    kmlelement.cpp \
    kmlgraphics.cpp \
    qmlimage.cpp \
    kmlrenderer.cpp

HEADERS += qtkml.h \
    rendervisitor.h \
    graphicsvisitor.h \
    stylevisitor.h \
    mercatorprojection.h \
    kmldocument.h \
    coord.h \
    kmlelement.h \
    kmlgraphics.h \
    kmlrenderer.h \
    qmlkml.h

#unix{
#    DESTDIR = ${PROJECT_LIBS}
#    #LIBS += -lexpat #if this is a static library this line must be in user's pro file
#}

#android{
#    API_VERSION = android-21
   # DESTDIR = ${PROJECT_LIBS}
    #IF NOT build, please check that Build Environent Set ANDROID_NDK_PLATFORM to android-21 !! (default android-9 does not compile)
#}

DESTDIR = $${PROJECT_LIBS}

message($$DESTDIR)

macx{
 #   QMAKE_MAC_SDK = macosx10.11
}


QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

#DISTFILES += \
#    third_party/third_party.pri

DISTFILES += \
    third_party/port.pri
