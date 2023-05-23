# QtKMLLib
Qt / QML Wrapper on libkml


brew install zlib
brew install minizip
brew install uriparser
brew install expat

There are three ways (due historical reasons) to render KML elements (pick one):
1) KmlItem QML item (preferred)
2) QImage from KmlDocument (slow but flexible)
3) QQuickImageProvider implementation KmlQmlImage from KmlQMLGraphics (see qmlkml.h) (also flexible)
4) KmlGraphics Elements + javascript composition via list of KmlQmlRenderer from KmlQmlGraphics  (see qmlkml.h) (fun with JS)

