#include "rendervisitor.h"
#include "mercatorprojection.h"
#include "qtkml.h"
#include <QPainter>
#include <QPainterPath>

using namespace QtKml;

RenderVisitor* RenderVisitor::create(const UrlDataProvider& urls, const CustomStyleProvider& styleProvider){
    Graphics::GraphicsList* graphics = new Graphics::GraphicsList;
    StyleList* style = new StyleList;
    RenderVisitor* rv = new RenderVisitor(graphics, style, urls, styleProvider);
    return rv;
}

RenderVisitor::RenderVisitor(Graphics::GraphicsList* graphics, StyleList* styles, const UrlDataProvider& urls, const CustomStyleProvider& styleProvider) :
    GraphicsVisitor(*graphics, *styles), m_urls(urls), m_styles(styleProvider), m_ptrGraphics(graphics), m_ptrStyles(styles){
}

RenderVisitor::~RenderVisitor(){
  //  delete m_ptrGraphics;
  //  delete m_ptrStyles;
}

/*
inline QGeoCoordinate operator-(const QGeoCoordinate& a, const QGeoCoordinate& b){
    return QGeoCoordinate(a.latitude() - b.latitude(), a.longitude() - b.longitude());
}

inline QGeoCoordinate operator+(const QGeoCoordinate& a, const QGeoCoordinate& b){
    return QGeoCoordinate(a.latitude() + b.latitude(), a.longitude() + b.longitude());
}
*/

/*
inline QPointF coordToPoint(const QGeoCoordinate& coord){
    qDebug() << coord;
    return QPointF(coord.latitude(), coord.longitude());
}
*/

void RenderVisitor::insertPoints(const Graphics::GraphicsPtr& g, const QSizeF size, const QPointF& center, qreal zoomFactor) {
    PointList points;
    MercatorProjection p;
    const auto vertices = g->vertices();
    for(int i = 0; i < vertices.length(); i++){
        const QPointF pos = p.fromLatLngToPoint(vertices[i].latitude(), vertices[i].longitude(), zoomFactor);
        points.append((pos - center) + QPointF(size.width() / 2.0, size.height() / 2.0));
    }
    m_polygons.insert(g.get(), points);
    for(const auto& gg: g->subgraphics())
        insertPoints(gg, size, center, zoomFactor);
}


void RenderVisitor::calculateProjection(const QSizeF size, const QPointF& center, qreal zoomFactor){
    //f_f = center;
    m_polygons.clear();
    for(const auto& g: m_graphics){
        insertPoints(g, size, center, zoomFactor);
    }
}

void RenderVisitor::drawPolygon(QPainter& painter, const Graphics::GraphicsPtr& g, const PointList& p) const{
    if(g->subgraphics().length() == 0)
        painter.drawPolygon(p.data(), p.length());
    else{
        QPainterPath path;
        const QPolygonF polygon(p);
        path.addPolygon(polygon);
        for(const auto& s : g->subgraphics()){
            const auto& hole = m_polygons.find(s.get()).value();
            QPainterPath inner;
            inner.addPolygon(hole);
            path = path.subtracted(inner);
        }
        painter.drawPath(path);
    }
}

void RenderVisitor::drawPoint(QPainter& painter, const StyleParams& s, const QPointF& p) const{
    //const QByteArray* bytes = m_urls.urlData(s.icon);
    const QByteArray* bytes = m_urls.urlData(s.icon());
    if(bytes != nullptr && bytes->length() > 0){
        auto image = QImage::fromData(*bytes);
        if(!image.isNull()){
            const QPoint pos(p.x() - image.width() / 2, p.y() - image.height());
            painter.drawImage(pos, image);
        }
    }
}



void RenderVisitor::render(QPainter& painter) const {
    for(const auto& g: m_graphics){
        const auto s = m_styles.customStyle(m_ptrStyles->get(g->styleId()), g->styleId());// = customStyles(m_ptrStyles->get(g->styleId()), g->styleId());
        QPen pen;
        pen.setColor(s.lineColor());
#ifdef ALLOW_QDEBUG_PRINT
        qDebug() << "Line color of " << g->styleId() << s.lineColor();
#endif
        pen.setWidth(s.lineWidth());
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        QBrush brush(s.fillColor(), s.isFill() ? Qt::BrushStyle::SolidPattern : Qt::NoBrush);
        painter.setBrush(brush);
        const auto& p = m_polygons.find(g.get()).value();
        switch(g->type()){
        case Graphics::Polygon:
            drawPolygon(painter, g, p);
            break;
        case Graphics::PolyLine:
            painter.drawPolyline(p.data(), p.length());
            break;
        case Graphics::Point:
            if(p.length() > 0)
                drawPoint(painter, s, p.at(0));
            break;
        default:
            break;
        }
    }
    //painter.drawEllipse(f_f, 20, 20);
}

/*
void RenderVisitor::VisitPolygon(const kmldom::PolygonPtr& element){
    if(element->has_outerboundaryis()){
        kmldom::OuterBoundaryIsPtr obound = element->get_outerboundaryis();
        if(obound->has_linearring()){
            setStyle();
            kmldom::LinearRingPtr ring = obound->get_linearring();
            kmldom::CoordinatesPtr coords = ring->get_coordinates();
            QVector<QPointF> points;
            for(size_t i = 0; i < coords->get_coordinates_array_size(); i++){
                kmlbase::Vec3 vec = coords->get_coordinates_array_at(i);

                GoogleMapsProjection p(640);
                const QPointF pos =  p.fromLatLngToPoint(vec.get_latitude(), vec.get_longitude(), m_zoomFactor);

                points.append((pos - m_pos) + QPointF(m_size.width() / 2.0, m_size.height() / 2.0));
                //qDebug() << px << py;
                //points.append(QPointF(px *  m_zoomFactor + m_size.width() / 2.0,
                //                      py * m_zoomFactor + m_size.height() / 2.0));
            }
            m_painter.drawPolygon(points.data(), points.length());
        }
    }
}
*/
