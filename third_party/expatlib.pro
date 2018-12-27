include("../config.pri")

QT -= gui
TARGET = expat
TEMPLATE = lib
CONFIG  += staticlib

CONFIG += c++11

QMAKE

INCLUDEPATH += port

#unix{
 #   DESTDIR = ../../../lib
    #LIBS += -lexpat #if this is a static library this line must be in user's pro file
#}

#android{
  #  INCLUDEPATH += port
  # DESTDIR = ../../android_lib
  #IF NOT build, please check that Build Environent Set ANDROID_NDK_PLATFORM to android-21 !! (default android-9 does not compile)
#}

DESTDIR = $${PROJECT_LIBS}

message($$DESTDIR)

macx{
   # QMAKE_MAC_SDK = macosx10.11
} 

DEFINES += HAVE_EXPAT_CONFIG_H
DEFINES += HAVE_MEMMOVE

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

HEADERS += \
    expat/lib/ascii.h \
    expat/lib/asciitab.h \
    expat/lib/expat.h \
    expat/lib/expat_external.h \
    expat/lib/iasciitab.h \
    expat/lib/internal.h \
    expat/lib/latin1tab.h \
    expat/lib/nametab.h \
    expat/lib/utf8tab.h \
    expat/lib/xmlrole.h \
    expat/lib/xmltok.h \
    expat/lib/xmltok_impl.h \
    expat/expat_config.h

SOURCES += \
    expat/lib/xmlparse.c \
    expat/lib/xmlrole.c \
    expat/lib/xmltok.c \
    expat/lib/xmltok_impl.c \
    expat/lib/xmltok_ns.c


