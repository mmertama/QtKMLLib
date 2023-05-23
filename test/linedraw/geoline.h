#ifndef GEOLINE_H
#define GEOLINE_H

#include "lineitem.h"
#include <QGeoCoordinate>

namespace LineDraw {

class GeoLine : public Line{
    Q_OBJECT
public:
    GeoLine(QObject* parent = nullptr);
    GeoLine(LinePrivate* priv, QObject* parent);
    bool appendVertex(const QGeoCoordinate& coord);
    template <template <typename T> class T >
    bool appendVertices(const T<QGeoCoordinate>& coords){
        for (const auto& v : coords)
            appendVertex(v);
        commit();
        return len() == coords.length();
    }
public slots:
    void reproject();
};

class GeoLines : public LineItem {
    Q_OBJECT
    Q_PROPERTY (QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY (qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY (qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
public:
    Q_INVOKABLE void remap();
    GeoLines(QQuickItem* parent = nullptr);
    QGeoCoordinate center() const;
    void setCenter(const QGeoCoordinate& center);
    qreal zoom() const;
    void setZoom(qreal zoom);
    Q_INVOKABLE GeoLine* geoPath(const QString& pathName);
    Q_INVOKABLE const GeoLine* geoPath(const QString& pathName) const;
    qreal lineWidth() const;
    void setLineWidth(qreal lineWidth);
    qreal metersToPixels(qreal meter) const;
    ~GeoLines() = default;
signals:
    void centerChanged();
    void zoomChanged();
    void lineWidthChanged();
    void geometryChanged();
    void geometryChangedAsync();
protected:
    Line* build();
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
private:
    QGeoCoordinate m_center;
    qreal m_zoom = 0;
    qreal m_lineWidth = 1; //1m !
};
}

Q_DECLARE_METATYPE(LineDraw::GeoLines*)

#endif // GEOLINE_H
