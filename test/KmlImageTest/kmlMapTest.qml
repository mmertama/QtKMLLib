import QtQuick 2.4

Rectangle {
    Connections{
        target: kmlgraphics
        onRenderersChanged:{
            if(kmlgraphics.renderers.length > 0){
                map.visibleRegion = kmlgraphics.renderers[0].bounds
            }
        }
    }
    color: "darkgrey"
    KmlMap{
        id: map
        anchors.fill:parent
    }
    Text{
        text: "KmlMap test (Javascript draw)"
    }
}

