#ifndef KMLQMLRENDERER_H
#define KMLQMLRENDERER_H

#include <QGeoRectangle>
#include "coord.h"
#include "graphicsvisitor.h"
#include "kmldocument.h"


namespace QtKml{

class KmlQmlElement;
class KmlDocument;

class KmlQmlRendererPrivate {
public:
    KmlQmlRendererPrivate(QObject* owner, const QSharedPointer<KmlDocumentPrivate>& doc/*, KmlDocument* parent*/);
    int polygonCount() const;
    const Graphics* graphics(int index) const;
    ~KmlQmlRendererPrivate();
   // const StyleParams& style(QString id) const;
    const Coord centerPoint() const {return m_centerPoint;}
    QGeoRectangle bounds() const {return m_doc->bounds();}
    const KmlDocumentPrivate* doc() const {return m_doc.data();}
    KmlDocumentPrivate* doc() {return m_doc.data();}
    const QVector<KmlQmlElement*>& elements() const {return m_list;}
    void freeMonitor(KmlDocumentMonitor* ptr) {if(m_doc != nullptr && m_doc->isMonitor(ptr)) m_doc->setMonitor(nullptr);}
private:
//    friend class KmlQmlRenderer;
    Coord m_centerPoint;
    QSharedPointer<KmlDocumentPrivate> m_doc;
    Graphics::GraphicsList m_graphics;
    StyleVisitor::StyleList m_styles;
    QVector<KmlQmlElement*> m_list;
};

} //namespace

#endif // KMLQMLRENDERER_H
