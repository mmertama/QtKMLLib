#include "qmlkml.h"
#include "kmlgraphics.h"
#include "mercatorprojection.h"

using namespace QtKml;

KmlItem::KmlItem(QQuickItem* parent) : QQuickPaintedItem(parent){
}

QGeoCoordinate KmlItem::center() const{
    return m_center;
}

void KmlItem::setCenter(const QGeoCoordinate& center){
    if(center != m_center){
        m_center = center;
        emit centerChanged(m_center);
        update();
    }
}

qreal KmlItem::zoom() const{
    return m_zoom;
}

void KmlItem::setZoom(qreal zoomVal){
    if(m_zoom != zoomVal){
        m_zoom = zoomVal;
        emit zoomChanged(m_zoom);
        update();
    }
}

KmlQmlGraphics* KmlItem::content() const{
    return m_graphics;
}

void KmlItem::setContent(KmlQmlGraphics* g){
    if(g != m_graphics){
        if(m_graphics != nullptr){
            QObject::disconnect(m_graphics, 0, this, 0);
        }
        m_graphics = g;
        QObject::connect(g, &KmlQmlGraphics::documentAdded, this, [this](const QString&){
            update();
        });
        QObject::connect(g, &KmlQmlGraphics::documentRemoved, this, [this](const QString&){
            update();
        });
        QObject::connect(g, &KmlQmlGraphics::renderersChanged, this, [this](){
            update();
        });
        emit contentChanged();
        update();
    }
}

QStringList KmlItem::idList() const{
    return m_idList.isEmpty() ? m_graphics->documents() : m_idList;
}

void KmlItem::setIdList(const QStringList& ids){
    if(ids != m_idList){
        m_idList = ids;
        emit idListChanged();
    }
}


void KmlItem::paint(QPainter* painter){
    if(m_graphics && m_graphics->d_ptr){
        auto  g = m_graphics->d_ptr.data();
        const MercatorProjection gmp;
        const auto centerPoint =  gmp.fromLatLngToPoint(m_center, m_zoom);
        const auto ids = idList();
        auto filter = [&ids](const QString& id){
            return ids.contains(id);
        };
        g->renderAll(*painter,
                     QRect(QPoint(0, 0), QSize(static_cast<int>(width()), static_cast<int>(height()))),
                     m_zoom, centerPoint, filter);
    }
}
