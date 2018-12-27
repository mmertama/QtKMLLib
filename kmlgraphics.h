#ifndef KMLGRAPHICS_H
#define KMLGRAPHICS_H

#include "kmldocument.h"
#include <QImage>
#include <QHash>
#include <functional>

namespace QtKml{

class KmlQmlRenderer;
class KmlQmlGraphics;

class KmlQmlGraphicsPrivate : KmlDocumentMonitor{
public:
    KmlQmlGraphicsPrivate(KmlQmlGraphics* parent) : q_ptr(parent){}
    int append(const QString& id, KmlQmlRenderer* renderer);
    int remove(const QString& id);
    bool contains(const QString& id) const {return m_renderers.contains(id);}
    int count() const {return m_renderers.count();}
    KmlQmlRenderer* at(int index) const;
    KmlQmlRenderer* get(const QString& id) const {return m_renderers[id];}
    void map(std::function<void (const KmlQmlRenderer& renderer)>) const;
  //  QVector<KmlQmlRenderer*>& renderers() { return m_list;}
  //  const QVector<KmlQmlRenderer*>& renderers() const { return m_list;}
    const QImage& image() const {return m_image;}
    void renderAll(const QSize& size, qreal zoom, const QPointF& midpoint);
   // QImage& image() {return m_image;}
    ~KmlQmlGraphicsPrivate();
    void documentDeleted(const QString& docId);
private:
    KmlQmlGraphics * const q_ptr;
    QImage m_image;
    QMap<QString, KmlQmlRenderer*> m_renderers;
    Q_DECLARE_PUBLIC(KmlQmlGraphics)
};

} //namespace

#endif // KMLGRAPHICS_H
