import QtQuick 2.6
import QtPositioning 5.5
import QtLocation 5.5

import com.corp.mars 1.0
import LineDraw 1.0

Rectangle {
    id: main
    Connections{
        target: kmlgraphics
        onRenderersChanged:{
            if(kmlgraphics.renderers.length > 0){
               map.visibleRegion = kmlgraphics.renderers[0].bounds
               kml.visible = false
               kml.reloader++
               kml.visible = true
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
    Image{
        id: kml
        anchors.fill: parent
        property int reloader: 0
        source: "image://kmlimage/center=" +
                map.center.latitude + "," + map.center.longitude +
                "&zoom=" + map.zoomLevel +
                "&size=" +  width + "x" + height +
                "&dummy=" + reloader
    }
     GeoLines{
         id: path
         anchors.fill: parent
         color: "#AA07FFFF"
         objectName: "workLines"
         thickness: 6
         visible: true
         antialiasing: true
         style: Line.STYLE_SOLID
     }
     /*
     Polymesh{
         id: poly
         objectName: "polymesh"
         anchors.fill: parent;
     } */
     Text{
         text: "mapTest test (image + LineDrawer)"
     }
}
