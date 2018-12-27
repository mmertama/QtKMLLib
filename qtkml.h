#ifndef QTKML_H
#define QTKML_H

#include <memory> //Android C++11 requires this
#include <QString>
#include <QSharedPointer>
#include <QObject>
#include <QVector>
#include <QVariant>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QStringList>

class QIODevice;

namespace QtKml {

class KmlDocumentPrivate;
class KmlElementPrivate;

/**
This is class to render KML data.

There are three ways to render KML elements:
1) QImage from KmlDocument
2) QQuickImageProvider implementation KmlQmlImage from KmlQMLGraphics (see qmlkml.h)
3) KmlGraphics Elements + javascript composition via list of KmlQmlRenderer from KmlQmlGraphics  (see qmlkml.h)

**/




#ifdef KMLCACHE
//Needs ziplib to support KMZ files
class KmlFetcher{
  public:
    virtual QByteArray fetchUrl(QString url) = 0;
};

#endif

class KmlElement{
public:
    class KmlVertex{
    public:
        qreal latitude;
        qreal longitude;
        int bound; // 0 < is outer bound, else holes
        operator QGeoCoordinate() const {return QGeoCoordinate(latitude, longitude);}
    };
    static constexpr int OUTBOUND = 0;
    typedef QVector<KmlVertex> KmlVertices;
    KmlElement(){}
    KmlElement(KmlElementPrivate* d);
    KmlElement(const KmlElement& other) : d_ptr(other.d_ptr){}
    KmlElement& operator=(const KmlElement& other) {d_ptr = other.d_ptr; return *this;}
    static constexpr auto POINT = "point";
    static constexpr auto POLYGON = "polygon";
    static constexpr auto POLYLINE = "polyline";
    static constexpr auto FILL = "fill";
    static constexpr auto FILL_COLOR = "fill_color";
    static constexpr auto LINE_COLOR = "line_color";
    static constexpr auto LINE_WIDTH = "line_width";
    static constexpr auto ICON = "icon";
    QString type() const;
    int boundCount() const;
    KmlVertices vertices(int bound) const;
    KmlVertices vertices() const;
    QGeoRectangle bounds() const;
    QGeoCoordinate center() const;
    QString style() const;
    QVariantMap styles() const;
    QString styleName() const;
    bool isIn(const QGeoCoordinate& coord) const;
private:
    std::shared_ptr<KmlElementPrivate>  d_ptr;
};

/**
 * @brief The KmlDocument class
 *
 * Represents KML data.
 *
 *
 */

class KmlDocument : public QObject{
    Q_OBJECT
public:
    KmlDocument(QObject* parent = NULL);
    ~KmlDocument();
#ifdef KMLCACHE
    bool open(QString url, KmlFetcher* fetcher);
#endif
    /**
     * @brief open
     * @param device
     * @param errorString - error information get from the XML parser
     * @return
     */
    bool open(QIODevice& device, QString* errorString = nullptr);
    /**
     * @brief open
     * @param document
     * @param errorString - error information get from the XML parser
     * @return
     */
    bool open(const QString& document, QString* errorString = nullptr);
    /**
     * @brief render, enforces image to be rerendered
     * @param image
     * @param size
     * @param zoom
     * @param centerPoint
     * @param erase
     */
    void render(QPixmap& image, const QSize& size, qreal zoom, const QPointF& centerPoint, bool erase);
    /**
     * @brief centerPoint - Calculates Mercator projected point of the coordinates at center of image
     * @param zoom
     * @return
     */
    QPointF centerPoint(qreal zoom) const;
    /**
     * @brief qmlImage - to register for QML
     * @return
     */
    QPixmap render(const QSize& size, qreal zoom);
    /**
     * @brief elements
     * @return
     */
    QVector<KmlElement> elements() const;
    /**
     * @brief project
     * @param coord map     coordinates
     * @param zoom          zoom level
     * @param centerPoint   centerPoint (see KmlDocument::centerPoint(qreal zoom))
     * @return
     */
    static QPointF project(const QGeoCoordinate &coord, qreal zoom, const QPointF& centerPoint);
    /**
     * @brief project
     * @param pos
     * @param zoom
     * @param centerPoint
     * @return
     */
    static QGeoCoordinate project(const QPointF& pos, qreal zoom, const QPointF& centerPoint);
    /**
     * @brief naturalZoom
     * @return
     */
    qreal naturalZoom(const QSize& size) const;
    /**
     * @brief isOpen
     * @return
     */
    bool isOpen() const;
    /**
     * @brief urlRequests
     * @return
     */
    QStringList urlRequests() const;
    /**
     * @brief setData - set external dat for URL, see setData
     * @param url
     * @param data
     * ...
    KmlDocument* doc = new KmlDocument(this);
    QFile file(filename);
    //Open file
    if(doc->open(file)){
        //Check if KML file has some HTTP requests (e.g. pin point images to retrieve)
        for(auto ss : doc->urlRequests()){
            //get external data
            QUrl url;
            url.setUrl(ss);
            QNetworkRequest request(url);
            QNetworkReply* reply =  m_accessManager->get(request);
            QObject::connect(reply, &QNetworkReply::finished, [reply, ss, doc](){
                doc->setData(ss, reply->readAll());
                reply->deleteLater();
            });
     ...
     */
    void setData(QString url, const QByteArray& data);
    /**
     * @brief setStyles
     * @param name
     * @param style
     */
    void setStyles(const QString& name, const QVariantMap& style);
    /**
     * @brief styles
     * @param name
     * @return
     */
    QVariantMap styles(const QString& name) const;
    /**
     * @brief styleNames
     * @return
     */
    QStringList styleNames() const;
signals:
    /**
     * @brief imageChanged - image is rendered
     */
    void imageChanged();
    /**
     * @brief documentChanged - content is updated, a render is needed
     */
    void documentChanged();
private:
    friend class KmlQmlGraphics;
    friend class KmlQmlImage;
    QSharedPointer<KmlDocumentPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(KmlDocument)
};

} //namespace

#endif // QTKML_H
