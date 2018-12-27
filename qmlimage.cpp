
#include <QtQml>
#include "qtkml.h"
#include "qmlkml.h"
#include "mercatorprojection.h"
#include "kmldocument.h"
#include "kmlgraphics.h"

using namespace QtKml;

KmlQmlImage::KmlQmlImage(KmlQmlGraphicsPrivate* g) : QQuickImageProvider(QQuickImageProvider::Pixmap), m_g(g){
}

KmlQmlImage::KmlQmlImage(KmlQmlImage && other) : QQuickImageProvider(QQuickImageProvider::Pixmap), m_g(other.m_g){
}

KmlQmlImage::KmlQmlImage(const KmlQmlImage & other) : QQuickImageProvider(QQuickImageProvider::Pixmap), m_g(other.m_g){
}

KmlQmlImage::~KmlQmlImage(){
}

KmlQmlImage& KmlQmlImage::operator=(const KmlQmlImage& other){
    m_g = other.m_g;
    return *this;
}


KmlQmlImage::KmlQmlImage() : QQuickImageProvider(QQuickImageProvider::Pixmap), m_g(nullptr){
}

QPair<qreal, qreal> parsePair(const QString& str, QChar separator){
    return QPair<qreal, qreal>(
                str.left(str.indexOf(QRegExp(separator))).toDouble(),
                str.mid(str.indexOf(QRegExp(separator)) + 1).toDouble());
}

QPixmap KmlQmlImage::requestPixmap(const QString& uri, QSize* size, const QSize& requestedSize){
    //qDebug() << "rpx" << uri;
    if(m_g == nullptr || m_g->count() <= 0){
        QSize sz(requestedSize.width() > 0
                 ? requestedSize.width() : 1, requestedSize.height() > 0 ? requestedSize.height() : 1);
        QPixmap image(sz);
        image.fill(Qt::transparent);
        *size = image.size();
        return image;
    }
    if(!uri.isEmpty() && (requestedSize == QSize(-1, -1) || m_g->image().size() != requestedSize)){
        QUrlQuery query(uri);
        const auto p = parsePair(query.queryItemValue("center"), ',');
        const qreal zoom = query.queryItemValue("zoom").toDouble();
        const auto s = parsePair(query.queryItemValue("size"), 'x');
        const auto fieldId = query.queryItemValue("fieldId");
        const qreal width = s.first;
        const qreal height = s.second;
        const MercatorProjection gmp;
        const QPointF centerPoint =  gmp.fromLatLngToPoint(p.first, p.second, zoom);
        const QSize sz = (requestedSize.width() > 0 && requestedSize.height() > 0) ?
                    requestedSize : QSize(qMax(1.0, width), qMax(1.0, height));
        const std::function<bool (const QString&)> filter =
                fieldId.isEmpty() ?
                    nullptr : std::function<bool (const QString&)>([fieldId](const QString& id)->bool{return fieldId == id;});
        m_g->renderAll(sz, zoom, centerPoint, filter);
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





