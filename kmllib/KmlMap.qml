import QtQuick 2.4
import QtPositioning 5.5
import QtLocation 5.5

Map {
    id: map
    anchors.fill: parent
    plugin: mapPlugin
    zoomLevel: 10.0
    center: QtPositioning.coordinate(63.0, 23.0)
    function setCenter(lat, lon){
        center = QtPositioning.coordinate(lat, lon)
    }

    Connections{
        target: kmlgraphics
        onGraphicsChanged:{
            console.log("graphics update");
            drawOverlays(kmlgraphics);
        }
    }

    function drawOverlays(){
        for(var g = 0; g < kmlgraphics.graphics.length; g++){
            var graphics = kmlgraphics.graphics[g]
            console.log("elements " + graphics.elements.length);
            for(var i = 0; i < graphics.elements.length; i++){
                var element = graphics.elements[i]
                console.log("center Point: " + element.center)
                switch(element.type){
                case "polygon":
                    var polygon = Qt.createQmlObject('import QtLocation 5.5; MapPolygon{smooth:true;antialiasing:true}', map, "mapPolygon")
                    if(element.styles["fill"])
                        polygon.color = _torealColor(element.styles["fill_color"])
                    polygon.border.width = element.styles["line_width"]
                    polygon.border.color = _torealColor(element.styles["line_color"])
                    _drawVertices(element, polygon)
                    map.addMapItem(polygon)
                    break
                case "polyline":
                    var polyline = Qt.createQmlObject('import QtLocation 5.5; MapPolyline{smooth:true;antialiasing:true}', map, "mapPolyline")
                    polyline.line.width = element.styles["line_width"]
                    polyline.line.color = _torealColor(element.styles["line_color"])
                    _drawVertices(element, polyline)
                    map.addMapItem(polyline)
                    break
                case "point":
                    var point = Qt.createQmlObject('import QtLocation 5.5; import QtQuick 2.4; MapQuickItem{
                                                    smooth:true;
                                                    antialiasing:true;
                                                    anchorPoint.x: p_icon.width / 2;
                                                    anchorPoint.y: p_icon.height;
                                                    sourceItem:Image{
                                                        id:p_icon
                                                        }
                                                    }', map, "mapQuickItem")
                    point.sourceItem.source = element.styles["icon"]
                    point.coordinate = QtPositioning.coordinate(element.vertices[0].latitude, element.vertices[0].longitude)
                    map.addMapItem(point)
                    console.log(element.styles["icon"] + " f4 " + point.coordinate + " --> " + point.sourceItem.source)
                    break;
                }
            }
        }
    }

    function _drawVertices(element, item){
        var vertices = element.vertices;
        for(var k = 0; k < vertices.length; k++){
            item.addCoordinate(QtPositioning.coordinate(vertices[k].latitude, vertices[k].longitude))
        }
    }


    function _torealColor(color){
        return Qt.rgba(
                    (0xFF & (color >> 16)) / 255.0,
                    (0xFF & (color >> 8)) / 255.0,
                    (0xFF & color) / 255.0,
                    (0xFF & (color >> 24)) / 255.0);
    }

    Plugin{
        id: mapPlugin
        name: "osm"
    }
}
