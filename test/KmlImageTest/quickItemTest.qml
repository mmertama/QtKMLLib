import QtQuick 2.6
import QtPositioning 5.5
import QtLocation 5.5

import com.corp.mars 1.0
import QtKML 1.0
import LineDraw 1.0

Rectangle {
    id: main
    Connections{
        target: kmlgraphics
        onRenderersChanged:{
            if(kmlgraphics.renderers.length > 0){
               map.visibleRegion = kmlgraphics.renderers[0].bounds
           }
        }
    }
    color: "darkgrey"
    Map{
        id: map
        objectName: "map";
        anchors.fill:parent
        plugin: mapPlugin
        zoomLevel: 10.0
        center: QtPositioning.coordinate(63.0, 23.0)
        function setCenter(lat, lon){
            center = QtPositioning.coordinate(lat, lon)
        }
        Plugin{
            id: mapPlugin
            name: "osm"
        }
        signal mapChanged()
        onZoomLevelChanged: mapChanged()
        onCenterChanged: mapChanged()
    }
    KmlItem{
        id: kml
        anchors.fill: parent
        content: kmlgraphics
        zoom: map.zoomLevel
        center: map.center
    }
     GeoLines{
         id: lines
         anchors.fill: parent
         color: "#AA07FFFF"
         objectName: "drawLines"
         lineWidth: 3
         visible: true
         antialiasing: true
         style: Line.STYLE_SOLID
         zoom: map.zoomLevel
         center: map.center
         opacity: 0.5
     }
     Text{
         text: "quickItemTest test (kmlItem + lineDrawer)"
     }
}
