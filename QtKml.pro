
include("../config.pri")

TEMPLATE = subdirs

SUBDIRS = kmllib qtKml kmlImageTest

android{
    SUBDIRS += expat
    expat.file = ./third_party/expatlib.pro
    kmllib.depends = expat
}

macx{
# do brew install expat
}

CONFIG += ordered

kmllib.file = ./third_party/libkml.pro
qtKml.file = ./QtKmlLib.pro
qtKml.depends = kmllib

# deprecated
#kmltest.file = ./test/KMLTest/KMLTest.pro
#kmltest.depends = qtKml

kmlImageTest.file = ./test/KmlImageTest/KmlImageTest.pro
kmlImageTest.depends = qtKml
