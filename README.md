# QtKMLLib
Qt / QML Wrapper on libkml

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



There are three ways (due historical reasons) to render KML elements (pick one):
1) KmlItem QML item (preferred)
2) QImage from KmlDocument (slow but flexible)
3) QQuickImageProvider implementation KmlQmlImage from KmlQMLGraphics (see qmlkml.h) (also flexible)
4) KmlGraphics Elements + javascript composition via list of KmlQmlRenderer from KmlQmlGraphics  (see qmlkml.h) (fun with JS)

