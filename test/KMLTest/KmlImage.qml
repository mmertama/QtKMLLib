import QtQuick 2.4

Item {
    property alias background : bg.source
    property alias kmlImage : canvas.data
    Image{
        id: bg
        anchors.fill: parent
    }
    Canvas{
        property variant data
        anchors.fill: parent
        onPaint:{
            if(data.length > 0)
                render()
        }
        function render(){
            var context = mycanvas.getContext("2d")
            var pos = 0;
            while(pos < data.length){
                var b = data[pos];
                var obj = b >> 16
                switch(obj){
                case 1:
                    pos = drawPolygon(context, pos, data);
                    break;
                }
            }
        }

        function toPoint(p){
            return p / ((1 << 31) / 360.0)
        }

        function drawPolygon(context, pos, data){
              var len = data[pos++] & 0xFFFF
              var d = data[pos++];
              var linew = d >> 16
              var fill = d & 0x1;
              var lineCol = data[pos++];
              var fillCol = data[pos++];
              console.log(len + " " + linew + fill)
              for(var i = 0; i < len; i++){
                  var px = toPoint(data[pos++])
                  var py = toPoint(data[pos++])
                  console.log(px + " " + py)
              }
        }
    }
}

