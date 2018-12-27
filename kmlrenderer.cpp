#include "kmlrenderer.h"
#include "kmldocument.h"
#include "qtkml.h"
#include "qmlkml.h"
#include "kmlelement.h"

//const StyleParams& KmlQmlRendererPrivate::style(QString id) const {
//    return m_styles.get(id);
//}

using namespace QtKml;

KmlQmlRendererPrivate::KmlQmlRendererPrivate(QObject* owner,
                                             const QSharedPointer<KmlDocumentPrivate>& doc/*,
                                             KmlDocument* parent*/):
    m_doc(doc){
    m_list.clear();
    m_graphics.clear();
    doc->getPolygons(m_graphics, m_styles);
    m_centerPoint = doc->centerPoint();
    for(auto g : m_graphics){
        auto p = new KmlQmlElementPrivate(doc->customStyle(m_styles[g->styleId()], g->styleId()), g);
        m_list.append(new KmlQmlElement(owner, p));
    }
}

KmlQmlRendererPrivate::~KmlQmlRendererPrivate(){
  //  for(auto g: m_graphics)
  //      delete g;
}

int KmlQmlRendererPrivate::polygonCount() const {
    return m_graphics.count();
}

const Graphics* KmlQmlRendererPrivate::graphics(int index) const{
    if(index < polygonCount()){
        return (m_graphics.at(index).get());
    }
    return NULL;
}

KmlQmlRenderer::KmlQmlRenderer(const QSharedPointer<KmlDocumentPrivate>& doc, KmlDocument* parent) : QObject(parent),
    d_ptr(new KmlQmlRendererPrivate(this, doc/*, parent*/)){
}

KmlQmlRenderer::~KmlQmlRenderer(){
}

QGeoRectangle KmlQmlRenderer::bounds() const{
    Q_D(const KmlQmlRenderer);
    return d->bounds();
}

QGeoCoordinate KmlQmlRenderer::center() const{
    Q_D(const KmlQmlRenderer);
    return QGeoCoordinate(d->centerPoint().lat(), d->centerPoint().lon());
}

QString KmlQmlRenderer::identifier() const{
    Q_D(const KmlQmlRenderer);
    return d->doc()->identifier();
}

QQmlListProperty<QObject> KmlQmlRenderer::elements(){
    const QQmlListProperty<KmlQmlElement>::CountFunction cf = [](QQmlListProperty<KmlQmlElement>* r)->int{
        return qobject_cast<KmlQmlRenderer*>(r->object)->d_ptr->elements().count();
    };
    const QQmlListProperty<KmlQmlElement>::AtFunction af = [](QQmlListProperty<KmlQmlElement>* r, int index)->KmlQmlElement*{
        return qobject_cast<KmlQmlRenderer*>(r->object)->d_ptr->elements().at(index);
    };
    return QQmlListProperty<QObject>(this, nullptr,
                                           reinterpret_cast<QQmlListProperty<QObject>::CountFunction>(cf),
                                           reinterpret_cast<QQmlListProperty<QObject>::AtFunction>(af));
}


void KmlQmlRenderer::setStyles(const QString& name, const QVariantMap& style){
    Q_D(KmlQmlRenderer);
    d->doc()->setStyles(name, style);
    emit documentChanged();
}

const QVariantMap& KmlQmlRenderer::styles(const QString& name) const{
    Q_D(const KmlQmlRenderer);
    return d->doc()->styles(name);
}

QStringList KmlQmlRenderer::styleNames() const{
    Q_D(const KmlQmlRenderer);
    return d->doc()->styleNames();
}


