
#include <QtQml>
#include "qtkml.h"
#include "qmlkml.h"
#include "mercatorprojection.h"
#include "kmldocument.h"
#include "kmlgraphics.h"

using namespace QtKml;

KmlQmlImage::KmlQmlImage(KmlQmlGraphicsPrivate* g) : QQuickImageProvider(QQuickImageProvider::Image), m_g(g){
}

KmlQmlImage::KmlQmlImage(KmlQmlImage && other) : QQuickImageProvider(QQuickImageProvider::Image), m_g(other.m_g){
}

KmlQmlImage::KmlQmlImage(const KmlQmlImage & other) : QQuickImageProvider(QQuickImageProvider::Image), m_g(other.m_g){
}

KmlQmlImage::~KmlQmlImage(){
}

KmlQmlImage& KmlQmlImage::operator=(const KmlQmlImage& other){
    m_g = other.m_g;
    return *this;
}


KmlQmlImage::KmlQmlImage() : QQuickImageProvider(QQuickImageProvider::Image), m_g(nullptr){
}

QPair<qreal, qreal> parsePair(const QString& str, QChar separator){
    return QPair<qreal, qreal>(
                str.left(str.indexOf(QRegExp(separator))).toDouble(),
                str.mid(str.indexOf(QRegExp(separator)) + 1).toDouble());
}

QImage KmlQmlImage::requestImage(const QString& uri, QSize* size, const QSize& requestedSize){
    if(m_g == nullptr || m_g->count() <= 0){
        const auto sz = requestedSize.width() < 0 || requestedSize.height() < 0 ? QSize(1, 1) : requestedSize;
        QImage image(sz, QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        *size = image.size();
        return image;
    }

    if((requestedSize == QSize(-1, -1) || m_g->image().size() != requestedSize)){
        QUrlQuery query(uri);
        const auto p = parsePair(query.queryItemValue("center"), ',');
        const qreal zoom = query.queryItemValue("zoom").toDouble();
        const auto s = parsePair(query.queryItemValue("size"), 'x');
        const qreal width = s.first;
        const qreal height = s.second;
        const MercatorProjection gmp;
        const QPointF centerPoint =  gmp.fromLatLngToPoint(p.first, p.second, zoom);
        const QSize sz = requestedSize == QSize(-1, -1) ? QSize(qMax(1.0, width), qMax(1.0, height)) : requestedSize;
        m_g->renderAll(sz, zoom, centerPoint);
#ifdef ALLOW_QDEBUG_PRINT
        qDebug() << "image updated";
#endif
    }
    *size = m_g->image().size();
#ifdef ALLOW_QDEBUG_PRINT
    qDebug() << "image returned" << *size;
#endif
    return m_g->image();
}





