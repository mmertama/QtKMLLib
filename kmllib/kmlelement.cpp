#include "kmlelement.h"
#include "mercatorprojection.h"
#include "qtkml.h"
#include "qmlkml.h"

using namespace QtKml;

QGeoRectangle KmlElementBasePrivate::bounds() const {
    kmlengine::Bbox box;
    kmlengine::GetGeometryBounds(m_g->domObject(), &box);
    return QGeoRectangle(QGeoCoordinate(box.get_north(), box.get_west()),
                                    QGeoCoordinate(box.get_south(), box.get_east()));
}

QGeoCoordinate KmlElementBasePrivate::centerPoint() const{
    qreal lat, lon;
    kmlengine::GetGeometryLatLon(m_g->domObject(), &lat, &lon);
    return QGeoCoordinate(lat, lon);
}

KmlElementBasePrivate::KmlElementBasePrivate(const StyleParams& styles, const Graphics::GraphicsPtr ptr) : m_g(ptr), m_styles(styles){
    /*m_styles.insert(KmlElement::FILL, styles.fill);
    m_styles.insert(KmlElement::FILL_COLOR, styles.fillColor);
    m_styles.insert(KmlElement::LINE_COLOR, styles.lineColor);
    m_styles.insert(KmlElement::LINE_WIDTH, styles.lineWidth);
    m_styles.insert(KmlElement::ICON, styles.icon.isNull() ? QString("") : styles.icon);*/
}


bool KmlElementBasePrivate::isIn(const QGeoCoordinate& coord) const{
    return polygon().containsPoint(QPointF(coord.latitude(), coord.longitude()), Qt::OddEvenFill);
}

KmlQmlElementPrivate::KmlQmlElementPrivate(const StyleParams& styles, const Graphics::GraphicsPtr ptr) : KmlElementBasePrivate(styles, ptr){
}

const QVariantList& KmlQmlElementPrivate::vertices() const{
    if(m_vertices.isEmpty()){
        KmlQmlElementPrivate* p = const_cast<KmlQmlElementPrivate*>(this);
        for(const auto& v : m_g->vertices())
            p->m_vertices.append(QVariant::fromValue(v));
    }
    return m_vertices;
}


const KmlElement::KmlVertices& KmlElementPrivate::vertices() const{
    if(m_vertices.isEmpty()){
        KmlElementPrivate* p = const_cast<KmlElementPrivate*>(this);
        p->m_bounds = 0;
        for(const auto& v : m_g->vertices()){
            p->m_vertices.append({v.latitude(), v.longitude(), m_bounds});
        }

        for(const auto& s : m_g->subgraphics()){
            for(const auto& v : s->vertices()){
                p->m_vertices.append({v.latitude(), v.longitude(), m_bounds});
            }
            ++p->m_bounds;
        }
    }
    return m_vertices;
}

int KmlElement::boundCount() const{
    return d_ptr->boundCount();
}

QString KmlElementBasePrivate::type() const{
    switch(m_g->type()){
    case Graphics::Point: return KmlElement::POINT;
    case Graphics::Polygon: return KmlElement::POLYGON;
    case Graphics::PolyLine: return KmlElement::POLYLINE;
    default: return QString("");
    }
}


QPolygonF KmlElementBasePrivate::polygon() const{
    QPolygonF polygon(m_g->vertices().size());
    int i = 0;
    for(const auto& coord : m_g->vertices()){
        polygon[i] = (QPointF(coord.latitude(), coord.longitude()));
        i++;
    }
    return polygon;
}

KmlQmlElement::KmlQmlElement(QObject* parent, KmlQmlElementPrivate* ren) : QObject(parent), d_ptr(ren){
}

KmlQmlElement::~KmlQmlElement(){
}

QGeoCoordinate KmlQmlElement::center() const{
    Q_D(const KmlQmlElement);
    return  d->centerPoint();
}

QGeoRectangle KmlQmlElement::bounds() const{
    Q_D(const KmlQmlElement);
    return  d->bounds();
}


QVariantMap KmlQmlElement::styles() const{
    Q_D(const KmlQmlElement);
    return d->styles();
}

QVariantList KmlQmlElement::vertices() const{
    Q_D(const KmlQmlElement);
    return d->vertices();
}

QString KmlQmlElement::type() const{
    Q_D(const KmlQmlElement);
    return d->type();
}

bool KmlQmlElement::isIn(const QGeoCoordinate& coord) const{
    Q_D(const KmlQmlElement);
    return d->isIn(coord);
}

QString KmlQmlElement::styleName() const{
    Q_D(const KmlQmlElement);
    return d->styleName();
}


KmlElementPrivate::KmlElementPrivate(const StyleParams& styles, const Graphics::GraphicsPtr ptr) :
    KmlElementBasePrivate(styles, ptr){
}

QString KmlElement::type() const{
    return d_ptr->type();
}

bool KmlElement::isIn(const QGeoCoordinate& coord) const{
    return d_ptr->isIn(coord);
}

KmlElement::KmlVertices KmlElement::vertices(int bound) const{
    KmlElement::KmlVertices vList;
    for(const auto v : d_ptr->vertices()){
        if(v.bound == bound)
            vList.append(v);
    }
    return vList;
}

int KmlQmlElement::boundCount() const{
    return d_ptr->boundCount();
}

KmlElement::KmlVertices KmlElement::vertices() const{
    return d_ptr->vertices();
}

QGeoRectangle KmlElement::bounds() const{
    return d_ptr->bounds();
}

QGeoCoordinate KmlElement::center() const{
    return d_ptr->centerPoint();
}

QVariantMap KmlElement::styles() const{
    return d_ptr->styles();
}

KmlElement::KmlElement(KmlElementPrivate *d) : d_ptr(d){
}

QString KmlElement::styleName() const{
    return d_ptr->styleName();
}
