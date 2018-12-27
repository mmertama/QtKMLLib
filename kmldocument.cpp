#include <math.h>

#include <QPainter>
#include <QtMath>

#include <QThread>

#include "kmldocument.h"
#include "kmlelement.h"
#include "qtkml.h"
#include "mercatorprojection.h"
#include "rendervisitor.h"

using namespace QtKml;

KmlDocumentPrivate::~KmlDocumentPrivate(){
    qDebug() << "~KmlDocumentPrivate" << this;
    if(m_monitor != nullptr)
        m_monitor->documentDeleted(m_identifier);
}

void KmlDocumentPrivate::getPolygons(Graphics::GraphicsList& graphics, StyleVisitor::StyleList& styles) const{
    if(m_root != nullptr){
        GraphicsVisitor visitor(graphics, styles);
        kmldom::SimplePreorderDriver(&visitor).Visit(m_root);
    }
}

kmldom::FeaturePtr KmlDocumentPrivate::asFeature(kmldom::ElementPtr element) const {
    const kmldom::KmlPtr kml = kmldom::AsKml(element);
    if(kml && kml->has_feature())
        return kml->get_feature();
    return kmldom::AsFeature(element);
}

QGeoRectangle KmlDocumentPrivate::bounds() const{
    kmlengine::Bbox box;
    kmlengine::GetFeatureBounds(rootFeature(), &box);
    return QGeoRectangle(QGeoCoordinate(box.get_north(), box.get_west()),
                                         QGeoCoordinate(box.get_south(), box.get_east()));
}

Coord KmlDocumentPrivate::centerPoint() const{
    const kmldom::FeaturePtr fp = asFeature(m_root);
    kmlengine::Bbox box;
    kmlengine::GetFeatureBounds(fp, &box);
    qreal lon, lat;
    box.GetCenter(&lat, &lon);
    return Coord(lat, lon);
    //MercatorProjection p;
    //return p.fromLatLngToPoint(lat, lon, zoom);
}


void KmlDocumentPrivate::renderAll(QPixmap& image, const QSize& size, qreal zoom, const QPointF& centerPoint, bool erase) {
    if(size != image.size()){
        image = QPixmap(size);
    }
    if(erase)
        image.fill(Qt::transparent);

    QPainter painter(&image);

    if(!painter.isActive()){
        qWarning() << "Painter failed" << image.size() << image.devicePixelRatio();
        return;
    }

    renderAll(painter, QRect({0, 0}, size), zoom, centerPoint);
}


void KmlDocumentPrivate::renderAll(QPainter& painter, const QRect& rect, qreal zoom, const QPointF& centerPoint){

    //Here we calculate bounding boxes to know if shapes are in then view port and it is feasible to render them
    const MercatorProjection mp;
    const auto b = bounds();
    const auto tl = mp.fromLatLngToPoint(b.topLeft(), zoom) - centerPoint + QPointF(rect.width() * 0.5, rect.height() * 0.5);
    const auto br = mp.fromLatLngToPoint(b.bottomRight(), zoom) - centerPoint + QPointF(rect.width() * 0.5, rect.height() * 0.5);
    const QRectF bbox(tl, br);
    const QRectF box(QPointF(0, 0), rect.size());

    if(box.intersects(bbox)){

        painter.setRenderHint(QPainter::Antialiasing);

        QScopedPointer<RenderVisitor> visitor(RenderVisitor::create(*this, *this));
        kmldom::SimplePreorderDriver(visitor.data()).Visit(m_root);
        const auto styles = visitor->styleList();
        for(const auto key : styles.keys()){
            StyleParams empty;
            if(!m_customStyles.contains(key))
                m_customStyles.insert(key, empty);
        }
        visitor->calculateProjection(rect.size(), centerPoint, zoom);
        visitor->render(painter);
    }
}



StyleParams KmlDocumentPrivate::customStyle(const StyleParams& def, const QString& styleId) const{
    if(!m_customStyles.contains(styleId))
        return def;
    StyleParams styles = def;
    styles.unite(m_customStyles[styleId]);
    return styles;
}

void KmlDocumentPrivate::setStyles(const QString& styleId, const QVariantMap& styleMap){
    if(!m_customStyles.contains(styleId)){
        StyleParams sp;
        m_customStyles.insert(styleId, sp);
    }
    m_customStyles[styleId].unite(styleMap);
/*
    StyleParams& sp = m_customStyles[name];
    if(styleMap.contains(KmlElement::LINE_WIDTH))
        sp.lineWidth = styleMap[KmlElement::LINE_WIDTH].toDouble();
    if(styleMap.contains(KmlElement::LINE_COLOR))
        sp.lineColor = styleMap[KmlElement::LINE_COLOR].value<QColor>();
    if(styleMap.contains(KmlElement::FILL_COLOR))
        sp.fillColor = styleMap[KmlElement::FILL_COLOR].value<QColor>();
    if(styleMap.contains(KmlElement::FILL))
        sp.fill = styleMap[KmlElement::FILL].toBool();
    if(styleMap.contains(KmlElement::ICON))
        sp.icon = styleMap[KmlElement::ICON].toString();

      qDebug() << "Style" << name << "changed" << sp.lineColor;*/
}

QVariantMap KmlDocumentPrivate::styles(const QString& name) const{
    return m_customStyles[name];
}

QStringList KmlDocumentPrivate::styleNames() const{
    return m_customStyles.keys();
}

KmlDocument::KmlDocument(QObject* parent) : QObject(parent), d_ptr(new KmlDocumentPrivate()){
    const auto emptyCache = [this](){
        Q_D(KmlDocument);
        d->elementCache().clear();
    };
    QObject::connect(this, &KmlDocument::documentChanged, emptyCache);
    QObject::connect(this, &KmlDocument::imageChanged, emptyCache);
}

KmlDocument::~KmlDocument(){
}

QPointF KmlDocument::centerPoint(qreal zoom) const{
    Q_D(const KmlDocument);
    const Coord c = d->centerPoint();
    MercatorProjection p;
    return p.fromLatLngToPoint(c.lat(), c.lon(), zoom);
}

void KmlDocument::render(QPixmap& image, const QSize& size, qreal zoom, const QPointF& centerPoint, bool erase){
    Q_D(KmlDocument);
    d->renderAll(image, size, zoom, centerPoint, erase);
    emit imageChanged();
}

qreal KmlDocument::naturalZoom(const QSize& size) const {
    Q_D(const KmlDocument);
    QGeoRectangle bounds = d->bounds();
    MercatorProjection p;
    const QPointF tl = p.fromLatLngToPoint(bounds.topLeft(), 1);
    const QPointF br = p.fromLatLngToPoint(bounds.bottomRight(), 1);
    const QPointF  mapSize = br - tl; //Im not sure if you can get map size like this... all of these qAbs are suspicious
    const qreal rel = qMax(qAbs(mapSize.x()) / qreal(size.width()), qAbs(mapSize.y()) / qreal(size.height()));
    const qreal zoom = qAbs(qLn(rel) / qLn(2.0)); //That is how I interpreted Google Maps documentation about "How zoom works"
    return zoom;
}

QPixmap KmlDocument::render(const QSize& size, qreal zoom){
    QPixmap image;
    render(image, size, zoom, centerPoint(zoom), true);
    return image;
}

QVector<KmlElement> KmlDocument::elements() const {
    Q_D(const KmlDocument);
    QVector<KmlElement>* elements = &(const_cast<KmlDocumentPrivate*>(d)->elementCache());
    if(elements->isEmpty()){
        Graphics::GraphicsList graphics;
        StyleVisitor::StyleList styles;
        d->getPolygons(graphics, styles);
        for(const auto g : graphics){
            KmlElementPrivate* p = new KmlElementPrivate(d->customStyle(styles[g->styleId()], g->styleId()), g);
            elements->append(KmlElement(p));
        }
    }
    return *elements;
}


#ifdef KMLCACHE
class Fetcher : public kmlbase::NetFetcher{
public:
    Fetcher(KmlFetcher* fetcher) : m_fetcher(fetcher){}
    bool FetchUrl(const std::string &url, std::string *data) const{
        *data = m_fetcher->fetchUrl(QString::fromStdString(url)).toStdString();
        return data->length() > 0;
    }
private:
    KmlFetcher* m_fetcher;
};

bool KmlDocument::open(QString url, KmlFetcher* fetcher){
    Q_D(KmlDocument);
    Fetcher f(fetcher);
    kmlengine::KmlCache kmlCache(&f, 1024 * 100);
    kmlengine::KmlFilePtr kmlFile = kmlCache.FetchKmlAbsolute(url.toUtf8().toStdString());
    const kmldom::ElementPtr root = kmlFile->get_root();
    if(root == nullptr)
        return false;
    d->setRoot(root);
    return true;
}

#endif

/*QImage KmlDocument::image() {
    Q_D(KmlDocument);
    return d->image();
}
*/

bool KmlDocument::open(QIODevice& device, QString* errorString){
    //Q_D(KmlDocument);
   // QByteArray data = device->readAll();
    const bool opened = device.isOpen();
    if(!opened)
        device.open(QIODevice::ReadOnly);
    const QString data = device.readAll();
    if(!opened)
        device.close();
    return open(data, errorString);
}

bool KmlDocument::open(const QString& document, QString* errorString){
    Q_D(KmlDocument);
#ifdef C_NUMBER_LOCALE
    auto systemLocale = ::setlocale(0, nullptr);
    ::setlocale(LC_NUMERIC, "C");
#endif
    std::string errors;
    const kmldom::ElementPtr root = kmldom::Parse(std::string(document.toUtf8().constData(), document.length()), &errors);
    if(errorString != nullptr && errors.length() > 0){
        *errorString = QString::fromStdString(errors);
    }
#ifdef C_NUMBER_LOCALE
    ::setlocale(LC_NUMERIC, systemLocale);
#endif
    if(root == nullptr){
        return false;
    }
    d->setRoot(root);
    return true;
}

QPointF KmlDocument::project(const QGeoCoordinate &coord, qreal zoom, const QPointF& centerPoint){
    MercatorProjection p;
    const QPointF pos = p.fromLatLngToPoint(coord, zoom);
    return pos - centerPoint;
}

QGeoCoordinate KmlDocument::project(const QPointF& pos, qreal zoom, const QPointF& centerPoint){
    MercatorProjection p;
    const QGeoCoordinate g = p.fromPointToLatLng(pos + centerPoint, zoom);
    return g;
}

bool KmlDocument::isOpen() const{
    Q_D(const KmlDocument);
    return d->rootFeature() != nullptr;
}


QStringList KmlDocument::urlRequests() const{
  Q_D(const KmlDocument);
  QStringList list;
  if(d->rootFeature() != nullptr){
      StyleVisitor::StyleList styles;

      QScopedPointer<StyleVisitor> visitor(new StyleVisitor(styles));
      kmldom::SimplePreorderDriver(visitor.data()).Visit(d->rootFeature());

      const auto values = styles.values();
      for(const auto s : values){
          if(!s.icon().isEmpty() && !d->m_data.contains(s.icon())){
              list.append(s.icon());
          }
      }
  }
  return list;
}

  void KmlDocument::setData(QString url, const QByteArray& data){
      Q_D(KmlDocument);
      d->m_data.insert(url, data);
      emit imageChanged();
  }

  void KmlDocument::setStyles(const QString& name, const QVariantMap& style){
      Q_D(KmlDocument);
      d->setStyles(name, style);
      emit documentChanged();
  }


 QVariantMap KmlDocument::styles(const QString& name) const{
    Q_D(const KmlDocument);
    return d->styles(name);
}

QStringList KmlDocument::styleNames() const{
    Q_D(const KmlDocument);
    return d->styleNames();
}

