#ifndef KMLDOCUMENT_H
#define KMLDOCUMENT_H

#include <QImage>
#include <QHash>
#include <QGeoRectangle>
#include "coord.h"
#include "rendervisitor.h"
#include "qtkml.h"
#include "graphicsvisitor.h"

namespace QtKml{

class KmlDocumentPrivate;

class KmlDocumentMonitor{
public:
    virtual void documentDeleted(const QString& id) = 0;
};

class KmlDocumentPrivate : public UrlDataProvider, public CustomStyleProvider{
public:
    KmlDocumentPrivate(){}
    ~KmlDocumentPrivate();
    void setRoot(kmldom::ElementPtr root){ m_root = root;}
    void renderAll(QImage& image, const QSize& size, qreal zoom, const QPointF& midpoint, bool erase);
    Coord centerPoint() const;
   // bool isPoint(const QPointF& pos) const {return pos == m_centerPoint;}
    void getPolygons(Graphics::GraphicsList& list, StyleVisitor::StyleList& styles) const;
    kmldom::FeaturePtr rootFeature() const {return asFeature(m_root);}
    QGeoRectangle bounds() const;
    const QByteArray* urlData(QString url) const {return m_data.contains(url) ? &(m_data.find(url).value()) : nullptr;}
    typedef QHash<QString, QByteArray> UrlData;
    QString identifier() const {return m_identifier;}
    void setIdentifier(const QString& id) {Q_ASSERT(m_identifier.isEmpty()); m_identifier = id;}
    void setStyles(const QString& name, const QVariantMap& styles);
    StyleParams customStyle(const StyleParams& defaultStyle, const QString& styleId) const;
    QVariantMap styles(const QString& name) const;
    QStringList styleNames() const;
    QVector<KmlElement>& elementCache() {return m_elementCache;}
    void setMonitor(KmlDocumentMonitor* monitor) {m_monitor = monitor;}
    bool isMonitor(KmlDocumentMonitor* monitor) const {return m_monitor == monitor;}
private:
    kmldom::FeaturePtr asFeature(kmldom::ElementPtr element) const;
private:
    friend class KmlDocument;
  //  QPointF m_centerPoint;
    kmldom::ElementPtr m_root;
    QHash<QString, QByteArray> m_data;
    QString m_identifier;
    QHash<QString, StyleParams> m_customStyles;
    QVector<KmlElement> m_elementCache;
    KmlDocumentMonitor* m_monitor = nullptr;
};

}

#endif // KMLDOCUMENT_H
