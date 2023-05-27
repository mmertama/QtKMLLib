# QtKMLLib

**The library to draw on top of map usinq geocoordinates.** 

## Introduction

Qt / QML Wrapper on libkml

The implementation is rip off from the old project, but I did a quick 
Qt6 port, may work with late Qt5 with minor modifications.

The original one was tested on Winodws, Mac, Linux and Android
This verision is tested on Mac and Linux, other versins are supposed
to work (as is or minor tweaks).

### KmlImageTest

The KmlImageTest is a mess (quick hack from the old project code) -
and terrible code, but there you can see examples way to use the library.

There are three ways (due historical reasons) to render KML elements (pick one):
* KmlItem QML item (preferred)
* QImage from KmlDocument (slow but flexible)
* QQuickImageProvider implementation KmlQmlImage from KmlQMLGraphics (see qmlkml.h) (also flexible)
* KmlGraphics Elements + javascript composition via list of KmlQmlRenderer from KmlQmlGraphics  (see qmlkml.h) (fun with JS)

You can find plenty of example KML files from the project's test folders.

If the application wont show the map data (KML is visible), the issues is likely with a missing SSL library.

## Building

If you didnt clone with --recursive flag, libkml may be missing - then 
just call 

git submodule update --init 

libkml, and hence QtKMLLib is dependant on several libraries

* zlib
* minizip
* boost
* uriparser
* expat


E.g. with brew

* brew install zlib
* brew install minizip
* brew install uriparser
* brew install expat

E.g. with apt-get 

* apt-get install libboost-all-dev
* apt-get install libminizip-dev
* apt-get install liburiparser-dev



