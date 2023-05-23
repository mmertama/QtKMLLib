#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QQmlContext>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QQmlProperty>

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmlkml.h"


#include "linepath.h"
#include "polymesh.h"
#include "lineitem.h"

#include "geoline.h"

constexpr char KML[]="KML";

using namespace QtKml;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){


    LineDraw::registerTypes();

    qmlRegisterType<Polymesh>("com.corp.mars", 1, 0, "Polymesh");
    qmlRegisterType<LineDraw::LineDrawer>("LineDraw", 1, 0, "LineDrawer");


    ui->setupUi(this);

    QObject::connect(ui->openFileBtn, &QPushButton::clicked, this, [this](){
        m_currentDoc = QFileDialog::getOpenFileName(this,
            "Open Kml", "", "Kml File (*.kml)", 0, QFileDialog::ReadOnly);
        if(!m_currentDoc.isEmpty()){
            emit fileChanged(m_currentDoc);
        }
    });

    QObject::connect(this, &MainWindow::fileChanged, this, &MainWindow::showKml);
    QObject::connect(ui->BorderColor, &QPushButton::clicked, [this](){this->showColorPicker(KmlElement::LINE_COLOR);});
    QObject::connect(ui->FillColor, &QPushButton::clicked, [this](){this->showColorPicker(KmlElement::FILL_COLOR);});

    m_kmlGraphics = new KmlQmlGraphics(this);
    ui->kmlMapQtQuick->rootContext()->setContextProperty("kmlgraphics", m_kmlGraphics);
    ui->kmlMapQtQuick->rootContext()->setContextProperty("application", this);

    ui->kmlMapQtQuick->setSource(QUrl("qrc:/kmlMapTest.qml"));

    ui->mapQtQuick->engine()->addImageProvider(QLatin1String("kmlimage"),  new KmlQmlImage(m_kmlGraphics->qmlImage()));
    ui->mapQtQuick->rootContext()->setContextProperty("kmlgraphics", m_kmlGraphics);
    ui->mapQtQuick->setSource(QUrl("qrc:/mapTest.qml"));
    m_accessManager = new QNetworkAccessManager();

    ui->quickItemMap->rootContext()->setContextProperty("kmlgraphics", m_kmlGraphics);
    ui->quickItemMap->setSource(QUrl("qrc:/quickItemTest.qml"));


    QObject::connect(m_kmlGraphics, &KmlQmlGraphics::documentAdded, [this](const QString& id){
       m_kmlId = id;
    });



    QObject::connect(m_kmlGraphics, &KmlQmlGraphics::renderersChanged, [this]{
        const QFileInfo kml(m_currentDoc);
        const auto json = kml.path() + '/' + kml.baseName() + ".json";
        if(QFileInfo::exists(json)){
            showJson(json, m_kmlId);
        }
        startLineDraw();
    });

    QObject::connect(&m_timer, &QTimer::timeout, [this](){
     //   qDebug() << "add point" << m_index;
        auto line0 = ui->quickItemMap->rootObject()->findChild<LineDraw::GeoLines*>("drawLines");
        auto line1 = ui->mapQtQuick->rootObject()->findChild<LineDraw::LineItem*>("workLines");
        Q_ASSERT(line0);
        Q_ASSERT(line1);
        auto noMorePoints = true;
        for(const auto& k : m_paths.keys()){
            const auto p = m_paths[k];
            if(m_index < p.length()){
                noMorePoints = false;
                auto path0 = line0->geoPath(k);
                auto path1 = line1->path(k);
                Q_ASSERT(path0);
                Q_ASSERT(path1);
             //   qDebug() << p[m_index];
                path0->appendVertex(p[m_index].first);
                path1->appendVertex(p[m_index].second);
            }
        }
        line1->commit();
        line0->commit();
        ++m_index;
        if(noMorePoints)
            m_timer.stop();
    });

    QObject::connect(ui->mapQtQuick, &QQuickWidget::statusChanged, [this](const QQuickWidget::Status& status) {
        if( status == QQuickWidget::Ready ) {
            Q_ASSERT(ui->mapQtQuick->rootObject());
            const auto mapItem = ui->mapQtQuick->rootObject()->findChild<QObject*>("map");
            connect(mapItem, SIGNAL(mapChanged()), this, SLOT(mapChanged()));
        }
    });



/*
    auto lines = ui->mapQtQuick->rootObject()->findChild<LinePath::LineDrawer*>("workLines");
    Q_ASSERT(lines);
    auto p = lines->append("workLines");
    Q_ASSERT(p);

    p->set({{100, 100}, {50, 100}, {100, 50}, {200, 200}});

    auto mesh = ui->mapQtQuick->rootObject()->findChild<Polymesh*>("polymesh");
    Q_ASSERT(mesh);
    *mesh = {{100, 100}, {50, 100}, {100, 50}, {200, 200}};
    */
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showColorPicker(const QString& item){
    KmlDocument* doc = m_kmlGraphics->document(KML);
    if( ! doc ) return;
    for(const auto& element : doc->elements()){
        if(element.type() == KmlElement::POLYGON){
            const auto styles = element.styles();
            const auto color = styles[item].value<QColor>();
            auto dlg = new QColorDialog(color, this);
            dlg->setOption(QColorDialog::ShowAlphaChannel, true);
            dlg->open();
            QObject::connect(dlg, &QColorDialog::colorSelected, [doc, element, item] (const QColor & color) mutable{
                doc->setStyles(element.styleName(), QVariantMap({{item, color}}));
            });
        }
    }
}

void MainWindow::showKml(const QString& filename){
    // Create a KML Document
    KmlDocument* doc = new KmlDocument(this);
    QFile file(filename);
    //Open file
    if(file.open(QFile::ReadOnly) && doc->open(file)){
        //Check if KML file has some HTTP requests (e.g. pin point images to retrieve)
        //This is optional
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
            QObject::connect(reply, &QNetworkReply::errorOccurred, [] (QNetworkReply::NetworkError code){
               qDebug() << "Download error " << code;
            });
        }
        //render and set as image
        const auto sz = ui->imageLabel->size();
        ui->imageLabel->setPixmap(doc->render(sz, doc->naturalZoom(sz)));
        //add for QML rendering
        m_kmlGraphics->append(doc, KML);
        QObject::connect(doc, &KmlDocument::documentChanged, [this, doc, sz](){
            ui->imageLabel->setPixmap(doc->render(sz, doc->naturalZoom((sz))));
        });
    }
    else{
        QMessageBox::warning(this, "Error", "Open Error");
        delete doc;
    }
}

void MainWindow::mapChanged(){
 /*   const  auto line0Item = ui->mapQtQuick->rootObject()->findChild<LinePath::LineItem*>("drawLines");
    const  auto line1Item = ui->mapQtQuick->rootObject()->findChild<LinePath::LineDrawer*>("workLines");
    const auto kmlDoc = m_kmlGraphics->document(m_kmlId);
    const auto el = kmlDoc->elements();
    const auto field = el[0];

    const auto mapItem = ui->mapQtQuick->rootObject()->findChild<QObject*>("map");
    const auto zoomLevel = QQmlProperty::read(mapItem, "zoomLevel").toDouble();
    const auto cmap = QQmlProperty::read(mapItem, "center").value<QGeoCoordinate>();
*//*
    auto cp = KmlDocument::project(cmap,  zoomLevel , QPoint(0, 0));

    auto centerPoint = KmlDocument::project(field.center(),  zoomLevel , QPointF(0, 0));
    centerPoint += QPointF(
                linesItem->width() * 0.5,
                linesItem->height() * 0.5);
    qDebug() << "cp" << cp << centerPoint << zoomLevel;
*/
  /*  linesItem->clear();
    for(const auto& key : m_paths.keys()){
        auto linePath = linesItem->path(key);
        auto path = m_paths[key];
        int index = 0;
        for(auto& c : path){
            c.second = QtKml::KmlDocument::project(c.first, zoomLevel, centerPoint);
            if(index < m_index)
                linePath->appendVertex(c.second);
        }
    }
    linesItem->commit();*/
}

void MainWindow::showJson(const QString& filename, const QString& id){
    QFile f(filename);
    if(f.open(QIODevice::ReadOnly)){
        m_paths.clear();
        const auto doc = QJsonDocument::fromJson(f.readAll());
        const auto obj = doc.object();
        const auto execution = obj["execution"].toObject();
        const auto paths = execution["paths"].toArray();

     //   const  auto line0Item = ui->quickItemMap->rootObject()->findChild<LinePath::LineDrawer*>("drawLines");
     //   Q_ASSERT(line0Item);

        const  auto line1Item = ui->mapQtQuick->rootObject()->findChild<LineDraw::LineItem*>("workLines");
        Q_ASSERT(line1Item);

        const auto mapItem = ui->mapQtQuick->rootObject()->findChild<QObject*>("map");
        Q_ASSERT(mapItem);

        const auto zoomLevel = QQmlProperty::read(mapItem, "zoomLevel").toDouble();

        const auto kmlDoc = m_kmlGraphics->document(id);
        const auto el = kmlDoc->elements();
        const auto field = el[0];

        auto centerPoint = KmlDocument::project(field.center(),  zoomLevel , QPointF(0, 0));
        centerPoint -= QPointF(line1Item->width() * 0.5, line1Item->height() * 0.5);

        for(const auto v : field.vertices()){
            const auto p = QtKml::KmlDocument::project(QGeoCoordinate(v), zoomLevel, centerPoint);
            qDebug() << v.longitude << v.latitude << "->" << p << zoomLevel;
            m_field.append(p);
        }


        for(const auto& v : paths){
            const auto o = v.toObject();
            const auto seederId = o["seederId"].toString();
            Q_ASSERT(!seederId.isEmpty());
            const auto executionDistance = o["executionDistance"].toString();
            const auto executionTime = o["executionTime"].toString();
            const auto path = o["path"].toArray();
            QList<QPair<QGeoCoordinate, QPointF>> coords;
            for(const auto& c : path){
                const auto a = c.toArray();
                const QGeoCoordinate g(a[0].toDouble(), a[1].toDouble());
                const auto p = QtKml::KmlDocument::project(g, zoomLevel, centerPoint);
              //  if(!m_field.containsPoint(p, Qt::OddEvenFill)){
                //    qDebug() << "warning" << p << " not inside of field";
              //  }
                coords.append({g, p});
            }
            m_paths.insert(seederId, coords);
        }
    }
}




void MainWindow::startLineDraw(){

    auto lines0 = ui->quickItemMap->rootObject()->findChild<LineDraw::GeoLines*>("drawLines");
    Q_ASSERT(lines0);

    auto lines1 = ui->mapQtQuick->rootObject()->findChild<LineDraw::LineItem*>("workLines");
    Q_ASSERT(lines1);

    for(const auto& pnames : m_paths.keys()){
        if(!lines1->path(pnames)){
            auto p = lines1->append(pnames);
            Q_ASSERT(p);
            p->setColor(QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF));   
        }
    }

    for(const auto& pnames : m_paths.keys()){
        if(!lines0->path(pnames)){
            auto p = lines0->append(pnames);
            Q_ASSERT(p);
            p->setColor(/*QColor(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF)*/Qt::gray);
        }
    }
    m_index = 0;
    m_timer.start(100);
}

