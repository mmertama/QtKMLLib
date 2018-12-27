import QtQuick 2.4
import QtQuick.Window 2.2
import QtWebView 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtPositioning 5.5

Window {
    visible: true
    width: 800
    height: 640
    Rectangle{
        id: mapPanel
        width: Math.min(parent.width, 640) //Free Google maps maximum size
        height: Math.min(parent.height, 640) //Free Google maps maximum size
        anchors.centerIn: parent
        color: "Red"
        property variant current: mapView
        property variant mapCenter: kmlgraphics.renderers[0].center
        property real zoom: 15.0
        function set(topView){
            console.log(current + " -> " + topView)

            if(topView !== current){
                current.hide = true
                current = topView
                current.hide = false
            }
        }

        MapContainer{
            id: webView
            anchors.fill: parent
            hide: false
            WebView{
                anchors.fill: parent
                url:"http://maps.googleapis.com/maps/api/staticmap?center=" + mapPanel.mapCenter.latitude + "," + mapPanel.mapCenter.longitude + "&zoom=" + mapPanel.zoom + "&size=" +  width + "x" + height + "&maptype=roadmap"
                onUrlChanged:
                    console.log(kmlgraphics.renderers[0].center + "--> " + url)
            }
        }
        MapContainer{
            id: mapView
            anchors.fill: parent
            hide: true
            KmlMap {
                id: map
                anchors.fill: parent
                center: mapPanel.mapCenter
                zoomLevel: mapPanel.zoom
            }
        }
        MapContainer{
            id: imageView
            anchors.fill: parent
            hide: true
            Image {
                id: image
                anchors.fill: parent
                asynchronous: true
                cache: true
                fillMode: Image.PreserveAspectCrop
                property string query: "center=" + mapPanel.mapCenter.latitude + "," + mapPanel.mapCenter.longitude + "&zoom=" + mapPanel.zoom + "&size=" +  width + "x" + height + "&maptype=roadmap"
                source:"http://maps.googleapis.com/maps/api/staticmap?" + query
            }
            Image{
                id: kml
                anchors.fill: parent
                source: "image://kmlimage/" + image.query;
            }
        }
    }
    RowLayout{
        ExclusiveGroup{
            id: mapSelectGroup
            onCurrentChanged:{
                if(current.target !== undefined)
                    mapPanel.set(current.target)
            }
        }

        RadioButton{
            id: webViewButton
            text: "Web View"
            exclusiveGroup: mapSelectGroup
            checked: true
            property variant target: webView
        }
        RadioButton{
            id: mapViewButton
            text: "Map View"
            exclusiveGroup: mapSelectGroup
            property variant target: mapView
        }
        RadioButton{
            id: imageViewButton
            text: "Image View"
            exclusiveGroup: mapSelectGroup
            property variant target: imageView
        }
    }
}
