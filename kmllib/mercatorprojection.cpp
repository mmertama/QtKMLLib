#include <QtMath>
#include <math.h>
#include "mercatorprojection.h"

using namespace QtKml;

MercatorProjection::MercatorProjection(int tilesz) : m_pixelOrigin(tilesz / 2.0, tilesz / 2.0),
    m_pixelsPerLonDegree(tilesz / 360.0),
    m_pixelsPerLonRadian(tilesz / (2.0 * M_PI)){
    }

QPointF MercatorProjection::fromLatLngToPoint(qreal lat, qreal lng, qreal zoom) const {
    const qreal px = m_pixelOrigin.x() + lng * m_pixelsPerLonDegree;
    // Truncating to 0.9999 effectively limits latitude to 89.189. This is
    // about a third of a tile past the edge of the world tile.

    const qreal siny = qMin(qMax(qSin(qDegreesToRadians(lat)), -0.9999), 0.9999);

    const qreal py = m_pixelOrigin.y() + 0.5 * qLn((1 + siny) / (1.0 - siny)) *- m_pixelsPerLonRadian;


    const qreal numTiles = qPow(2, zoom);
    return QPointF(px * numTiles, py * numTiles);
}


QGeoCoordinate MercatorProjection::fromPointToLatLng(const QPointF& point, qreal zoom) const{
    qreal clat = point.x();
    qreal clon = point.y();
    const qreal numTiles = qPow(2, zoom);
    clat /= numTiles;
    clon /= numTiles;

    const qreal lng = (clat - m_pixelOrigin.x()) / m_pixelsPerLonDegree;

    const qreal latRadians = (clon - m_pixelOrigin.y()) / - m_pixelsPerLonRadian;
    const qreal lat = qRadiansToDegrees(2 * qAtan(qExp(latRadians)) - M_PI / 2.0);
    return QGeoCoordinate(lat, lng);
    }


