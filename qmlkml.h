#ifndef QMLKML_H
#define QMLKML_H

#include "qtkml.h"
#include <QQuickImageProvider>
#include <QQmlListProperty>
#include <QQmlPropertyMap>


namespace QtKml {

class KmlQmlRendererPrivate;
class KmlQmlElementPrivate;
class KmlQmlGraphicsPrivate;


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
    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
private:
    KmlQmlGraphicsPrivate* m_g;
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
public:
    QGeoRectangle bounds() const;
    QVariantMap styles() const;
    QVariantList vertices() const;
    QGeoCoordinate center() const;
    QString type() const;
    QString styleName() const;
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
    Q_PROPERTY(QGeoRectangle bounds READ bounds)
    Q_PROPERTY(QString identifier READ identifier)
public:
    Q_INVOKABLE void setStyles(const QString& name, const QVariantMap& style);
    Q_INVOKABLE const QVariantMap& styles(const QString& name) const;
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
    bool append(KmlDocument* document, QString id = "");
    /**
     * @brief qmlImage
     * @return
     */
    KmlQmlImage qmlImage();
    /**
     * @brief remove
     * @param document
     */
    void remove(KmlDocument* document = nullptr, QString id = "");
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
signals:
    void documentAdded(QString id);
    void documentRemoved(QString id);
    void renderersChanged();
private:
    QScopedPointer<KmlQmlGraphicsPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KmlQmlGraphics)
};

} //namespace QtKml
Q_DECLARE_METATYPE(QtKml::KmlQmlRenderer*)
Q_DECLARE_METATYPE(QtKml::KmlQmlElement*)

#endif // QMLKML_H

