#include "geoline.h"
#include "qtkml.h"
#include "line_p.h"

using namespace LineDraw;

void LineDraw::registerTypes(){
    qmlRegisterType<LineDraw::Line>("LineDraw", 1, 0, "Line");
    qmlRegisterType<LineDraw::LineItem>("LineDraw", 1, 0, "LineItem");
    qmlRegisterType<LineDraw::GeoLines>("LineDraw", 1, 0, "GeoLines");
}


class GeoLinePriv : public LinePrivate{
public:
    GeoLinePriv(GeoLines* gg, LinePainter* d) : LinePrivate(d), gl(gg){}
    QList<QGeoCoordinate> coords;
    GeoLines* gl;
};

GeoLine::GeoLine(QObject* parent) : Line(parent){
    QObject::connect(this, &Line::verticesCountChanged, [this](){
        if(verticesCount() == 0){
            auto priv = static_cast<GeoLinePriv*>(m_p.data());
            priv->coords.clear();
        }
    });
}

GeoLine::GeoLine(LinePrivate* priv, QObject* parent) : Line(priv, parent){
}

bool GeoLine::appendVertex(const QGeoCoordinate& coord) {
    if(coord.isValid()){
        auto p = static_cast<GeoLinePriv*>(m_p.data());
        auto centerPoint = QtKml::KmlDocument::project(p->gl->center(),
                                                   p->gl->zoom() , QPointF(0, 0));
        centerPoint -= QPointF(p->gl->width() * 0.5, p->gl->height() * 0.5);

        const auto vertex = QtKml::KmlDocument::project(coord, p->gl->zoom(), centerPoint);
        p->coords.append(coord);
        Line::appendVertex(vertex);
        return true;
    }
    return false;
}


qreal GeoLines::metersToPixels(qreal meter) const {
    const QGeoCoordinate g0(0, 0);
    const QGeoCoordinate g1 = g0.atDistanceAndAzimuth(meter, 0);
    Q_ASSERT(0 == (g0.longitude() - g1.longitude())); //as up (0)
    const auto metersAsDegree = qAbs(g0.latitude() - g1.latitude());
    const auto worldPosition = center();
    auto centerPoint = QtKml::KmlDocument::project(center(),
                                                   zoom() , QPointF(0, 0));
    centerPoint -= QPointF(width() * 0.5, height() * 0.5);
    const auto offsetFromCenter = worldPosition.latitude() + metersAsDegree;
    const auto point = QtKml::KmlDocument::project(QGeoCoordinate(offsetFromCenter, worldPosition.longitude()), zoom(), centerPoint)
            - QPointF(width() * 0.5, height() * 0.5);
    return qAbs(point.y());
}

void GeoLine::reproject(){
    auto p = static_cast<GeoLinePriv*>(m_p.data());
    if(p->coords.count() == verticesCount()){
        auto centerPoint = QtKml::KmlDocument::project(p->gl->center(),
                                                       p->gl->zoom() , QPointF(0, 0));
        centerPoint -= QPointF(p->gl->width() * 0.5, p->gl->height() * 0.5);

        for(auto i = 0; i < verticesCount() ; i++){
            const auto vertex = QtKml::KmlDocument::project(p->coords[i], p->gl->zoom(), centerPoint);
            p->vertices[i] = vertex;
        }
        emit lineChanged();
    }
}


void GeoLines::remap(){
    emit geometryChanged();
}


void GeoLines::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry){
    LineItem::geometryChanged(newGeometry, oldGeometry);
    emit geometryChanged();
}

Line* GeoLines::build(){
    auto p = new GeoLine(new GeoLinePriv(this, this), this);
    QObject::connect(p, &Line::lineChanged, this, &LineItem::pathsChanged);
    QObject::connect(this, &LineItem::styleChanged, p, &Line::setStyle);
    QObject::connect(this, &LineItem::colorChanged, p, &Line::setColor);
    QObject::connect(this, &LineItem::thicknessChanged, p, &Line::setThickness);
    QObject::connect(this, &GeoLines::zoomChanged, p, &GeoLine::reproject);
    QObject::connect(this, &GeoLines::centerChanged, p, &GeoLine::reproject);
    QObject::connect(this, static_cast<void (GeoLines::*)()>(&GeoLines::geometryChanged), p, &GeoLine::reproject);
    QObject::connect(this, &GeoLines::geometryChangedAsync, p, &GeoLine::reproject, Qt::QueuedConnection);
    const auto slotOfLineChange = [this](){
        setThickness(metersToPixels(m_lineWidth));
    };
    QObject::connect(this, &GeoLines::lineWidthChanged, slotOfLineChange);
    QObject::connect(this, &GeoLines::zoomChanged, slotOfLineChange);
    QObject::connect(this, &GeoLines::centerChanged,slotOfLineChange);

    setThickness(metersToPixels(m_lineWidth));

    return p;
}

GeoLines::GeoLines(QQuickItem* parent) : LineItem (parent){
}

QGeoCoordinate GeoLines::center() const{
    return m_center;
}

void GeoLines::setCenter(const QGeoCoordinate& center){
    if(m_center != center){
        m_center = center;
        emit centerChanged();
    }
}

qreal GeoLines::zoom() const{
    return m_zoom;
}

void GeoLines::setZoom(qreal zoom){
    if(m_zoom != zoom){
        m_zoom = zoom;
        emit zoomChanged();
    }
}

qreal GeoLines::lineWidth() const{
    return m_lineWidth;
}

void GeoLines::setLineWidth(qreal lineWidth){
    if(lineWidth != m_lineWidth){
        m_lineWidth = lineWidth;
        emit lineWidthChanged();
    }
}

GeoLine* GeoLines::geoPath(const QString& pathName){
    return qobject_cast<GeoLine*> (path(pathName));
}

const GeoLine* GeoLines::geoPath(const QString& pathName) const{
    return qobject_cast<const GeoLine*> (path(pathName));
}

