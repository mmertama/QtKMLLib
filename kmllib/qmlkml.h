#ifndef QMLKML_H
#define QMLKML_H

#include "qtkml.h"
#include <QQuickImageProvider>
#include <QQmlListProperty>
#include <QQmlPropertyMap>
#include <QQuickPaintedItem>


namespace QtKml {

class KmlQmlRendererPrivate;
class KmlQmlElementPrivate;
class KmlQmlGraphicsPrivate;
class KmlQmlGraphics;


/**
 * @brief The KmlQmlImage class
 *
 * Register QmlImage before loading QML file e.g.
 * QQmlApplicationEngine engine;
 * engine.addImageProvider(QLatin1String("kmlimage"),  new KmlQmlImage(graphics->qmlImage()));
 *
 * The rendering is defined with coordinates, zoom and size as analogous as Google static maps parameter syntax, that
 * makes easy to composite rendering with maps.
 *
 * Image{
 *      id: kml
 *      anchors.fill: parent
 *      source: "image://kmlimage/center=" + latitude + "," + longitude + "&zoom=" + zoom + "&size=" +  width + "x" + height"
 *       }
 *
 */

class KmlQmlImage : public QQuickImageProvider{
public:
    KmlQmlImage(KmlQmlImage && other);
    KmlQmlImage(const KmlQmlImage& other);
    KmlQmlImage& operator=(const KmlQmlImage& other);
    KmlQmlImage();
    KmlQmlImage(KmlQmlGraphicsPrivate* graphics);
    ~KmlQmlImage();
private:
    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);
private:
    KmlQmlGraphicsPrivate* m_g;
};

/**
 * @brief The KmlItem class
 */
class KmlItem : public QQuickPaintedItem{
    Q_OBJECT
    Q_PROPERTY (QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY (qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY (KmlQmlGraphics* content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QStringList elements READ idList WRITE setIdList NOTIFY idListChanged)
public:
    KmlItem(QQuickItem* parent = nullptr);
    QGeoCoordinate center() const;
    void setCenter(const QGeoCoordinate& center);
    qreal zoom() const;
    void setZoom(qreal zoom);
    KmlQmlGraphics* content() const;
    void setContent(KmlQmlGraphics* c);
    QStringList idList() const;
    void setIdList(const QStringList& ids);
signals:
    void centerChanged(const QGeoCoordinate& coord);
    void zoomChanged(qreal zoom);
    void contentChanged();
    void idListChanged();
private:
    void paint(QPainter* painter);
private:
    QGeoCoordinate m_center;
    qreal m_zoom = 0;
    KmlQmlGraphics* m_graphics = nullptr;
    QStringList m_idList;
};

/**
 * @brief The KmlQmlElement class
 *
 * Impements a KML type for QML rendering
 * (There is no C++ Qt Geo grapics and therefore the actual rendering is done using Javascript)
 * That is done un KmlMap.QML and this this class shall not beed to be called directly
 *
 */

class KmlQmlElement : public QObject{
    Q_OBJECT
public:
    KmlQmlElement(QObject* parent, KmlQmlElementPrivate* p);
    ~KmlQmlElement();
private:
    Q_PROPERTY(QGeoCoordinate center READ center)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QGeoRectangle bounds READ bounds)
    Q_PROPERTY(QVariantMap styles READ styles)
    Q_PROPERTY(QVariantList vertices READ vertices)
    Q_PROPERTY(QString styleName READ styleName)
    Q_PROPERTY(int boundCount READ boundCount)
public:
    QGeoRectangle bounds() const;
    QVariantMap styles() const;
    QVariantList vertices() const;
    QGeoCoordinate center() const;
    QString type() const;
    QString styleName() const;
    int boundCount() const;
public:
    Q_INVOKABLE bool isIn(const QGeoCoordinate& coord) const;
private:
    QScopedPointer<KmlQmlElementPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KmlQmlElement)
};

/**
 * @brief The KmlQmlRenderer class
 *
 * Impements a KML type for QML rendering
 * (There is no C++ Qt Geo grapics and therefore the actual rendering is done using Javascript)
 * That is done un KmlMap.QML and this this class shall not beed to be called directly
 *
 */

class KmlQmlRenderer: public QObject{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> elements READ elements NOTIFY elementsChanged)
    Q_PROPERTY(QGeoCoordinate center READ center NOTIFY centerChanged)
    Q_PROPERTY(QGeoRectangle bounds READ bounds NOTIFY boundsChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)
public:
    Q_INVOKABLE void setStyles(const QString& name, const QVariantMap& style);
    Q_INVOKABLE QVariantMap styles(const QString& name) const;
    Q_INVOKABLE QStringList styleNames() const;
public:
    KmlQmlRenderer(const QSharedPointer<KmlDocumentPrivate>& doc, KmlDocument* parent);
    ~KmlQmlRenderer();
    QQmlListProperty<QObject> elements();
    QGeoCoordinate center() const;
    QGeoRectangle bounds() const;
    QString identifier() const;
signals:
    void elementsChanged();
    void centerChanged();
    void documentChanged();
    void boundsChanged();
    void identifierChanged();
private:
    friend class KmlQmlGraphics;
    friend class KmlQmlGraphicsPrivate;
    QScopedPointer<KmlQmlRendererPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KmlQmlRenderer)
};


/**
 * @brief The KmlQmlGraphics class
 *
 * Impements a KML type for QML rendering
 *
 * Registered and add KML documents for rendering
 *
 *   QFile kml(":/alcatea.kml");
 *   kml.open(QIODevice::ReadOnly | QIODevice::Text);
 *   QScopedPointer<KmlDocument> document(new KmlDocument());
 *   QScopedPointer<KmlQmlGraphics> graphics(new KmlQmlGraphics());
 *   document->open(&kml);
 *   kml.close();
 *   graphics->append(document.data());
 *   engine.rootContext()->setContextProperty("kmlgraphics", graphics.data());
 *
 */

class KmlQmlGraphics: public QObject{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> renderers READ renderers NOTIFY renderersChanged)
    Q_PROPERTY(QGeoRectangle bounds READ bounds)
public:
    Q_INVOKABLE QVariant renderer(const QString& id) const;
public:
    KmlQmlGraphics(QObject* parent = NULL);
    ~KmlQmlGraphics();
    /**
     * @brief append for QML rendering
     * @param document
     */
    bool append(KmlDocument* document, const QString& id = "");
    /**
     * @brief qmlImage
     * @return
     */
    KmlQmlImage qmlImage();
    /**
     * @brief remove
     * @param document
     */
    void remove(KmlDocument* document = nullptr, const QString& id = "");
    /**
     * @brief graphics
     * @return
     */
    QQmlListProperty<QObject> renderers();
    /**
     * @brief bounds
     * @return
     */
    QGeoRectangle bounds() const;
    /**
     * @brief document
     * @param id
     * @return
     */
    KmlDocument* document(const QString& id) const;

    /**
     * @brief documents
     * @return
     */
    QStringList documents() const;
signals:
    void documentAdded(const QString& id);
    void documentRemoved(const QString& id);
    void renderersChanged();
private:
    QScopedPointer<KmlQmlGraphicsPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KmlQmlGraphics)
    friend class KmlItem;
};

} //namespace QtKml
Q_DECLARE_METATYPE(QtKml::KmlQmlRenderer*)
Q_DECLARE_METATYPE(QtKml::KmlQmlElement*)

#endif // QMLKML_H

