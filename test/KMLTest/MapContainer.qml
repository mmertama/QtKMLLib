import QtQuick 2.0

Rectangle {
    property bool hide: false
    color: "Grey"
    visible: opacity > 0.0
    onHideChanged: opacity = hide ? 0.0 : 1.0
    NumberAnimation on opacity{
        duration: 200
        easing.type: Easing.InOutQuad
    }
}

