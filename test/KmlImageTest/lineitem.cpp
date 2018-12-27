#include "lineitem.h"
#include "line_p.h"

using namespace LineDraw;

//STYLE_SOLID, STYLE_DASH, STYLE_DOTS
const Qt::PenStyle styleMap[] = {Qt::SolidLine, Qt::DashLine, Qt::DotLine};

LineItem::LineItem(QQuickItem* parent) : QQuickPaintedItem(parent){
    QObject::connect(this, &LineItem::pathsChanged, [this](){
       update();
    });

}


 void LineItem::paint(QPainter* painter){
     painter->setRenderHint(QPainter::Antialiasing, true);
     for(const auto line : m_lines){
         const auto color = line->color();
         const auto style = line->style();
         const auto thickness = line->thickness();
         const QPen pen(QBrush(color),
                    thickness,
                    styleMap[style],
                    Qt::RoundCap,
                    Qt::RoundJoin);
         painter->setPen(pen);
         const auto vertices = line->vertices();
         painter->drawPolyline(vertices.data(), line->verticesCount());

     }
}


 Line* LineItem::build(){
    auto p = new Line(new LinePrivate(this), this);
    QObject::connect(p, &Line::lineChanged, this, &LineItem::pathsChanged);
    QObject::connect(this, &LineItem::styleChanged, p, &Line::setStyle);
    QObject::connect(this, &LineItem::colorChanged, p, &Line::setColor);
    QObject::connect(this, &LineItem::thicknessChanged, p, &Line::setThickness);
    return p;
 }

