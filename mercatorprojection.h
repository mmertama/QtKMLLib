#ifndef MERCATORPROJECTION_H
#define MERCATORPROJECTION_H

#include <QtCore>
#include <QGeoCoordinate>

constexpr int MERCATOR_RANGE = 256;

namespace QtKml {

class MercatorProjection {
public:
    MercatorProjection(int range = MERCATOR_RANGE);
    QPointF fromLatLngToPoint(qreal lat, qreal lng, qreal zoom) const;
    QPointF fromLatLngToPoint(const QGeoCoordinate& coord, qreal zoom) const {
        return fromLatLngToPoint(coord.latitude(), coord.longitude(), zoom);}
    QGeoCoordinate fromPointToLatLng(const QPointF& point, qreal zoom) const;
private:
    QPointF m_pixelOrigin;
    qreal m_pixelsPerLonDegree;
    qreal m_pixelsPerLonRadian;
};

}

#endif // MERCATORPROJECTION_H
