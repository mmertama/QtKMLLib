import QtQuick 2.0
import QtPositioning 5.5
import QtLocation 5.5

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
        onZoomLevelChanged:
            console.log("zoomlevel " + zoomLevel)
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
}
