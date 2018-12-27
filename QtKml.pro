
TEMPLATE = subdirs

SUBDIRS = kmllib qtKml kmltest kmlImageTest

android{
    SUBDIRS += expat
    expat.file = ./third_party/expatlib.pro
    kmllib.depends = expat
}

CONFIG += ordered

kmllib.file = ./third_party/libkml.pro
qtKml.file = ./QtKmlLib.pro
qtKml.depends = kmllib

kmltest.file = ./test/KMLTest/KMLTest.pro
kmltest.depends = qtKml

kmlImageTest.file = ./test/KmlImageTest/KmlImageTest.pro
kmlImageTest.depends = qtKml
