#ifndef KMLELEMENT_H
#define KMLELEMENT_H

#include <QGeoRectangle>
#include <QQmlPropertyMap>
#include "graphicsvisitor.h"
#include "coord.h"
#include "qtkml.h"

namespace QtKml{


class KmlQmlRendererPrivate;
class Graphics;

class KmlElementBasePrivate{
public:
    KmlElementBasePrivate(const StyleParams& styles, const Graphics::GraphicsPtr ptr);
    QVariantMap styles() const {return m_styles;}
    QString type() const;
    QString styleName() const {return m_g->styleId();}
    QGeoCoordinate centerPoint() const;
    QGeoRectangle bounds() const;
    QPolygonF polygon() const;
    bool isIn(const QGeoCoordinate& coord) const;
protected:
    const Graphics::GraphicsPtr m_g;
    QVariantMap m_styles;
};

class KmlQmlElementPrivate : public KmlElementBasePrivate{
public:
    KmlQmlElementPrivate(const StyleParams& styles, const Graphics::GraphicsPtr ptr);
    const QVariantList& vertices() const;
private:
    QVariantList m_vertices;
};

class KmlElementPrivate : public KmlElementBasePrivate{
public:
    KmlElementPrivate(const StyleParams& params, const Graphics::GraphicsPtr ptr);
    const KmlElement::KmlVertices& vertices() const;
private:
    KmlElement::KmlVertices m_vertices;
};

} //namespace

#endif // KMLELEMENT_H
