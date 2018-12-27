#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QQmlContext>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmlkml.h"

constexpr char KML[]="KML";

using namespace QtKml;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    QObject::connect(ui->openFileBtn, &QPushButton::clicked, this, [this](){
        QString filename = QFileDialog::getOpenFileName(this,
            "Open Kml", "", "Kml File (*.kml)", nullptr, QFileDialog::ReadOnly);
        if(!filename.isEmpty()){
            emit fileChanged(filename);
        }
    });

    QObject::connect(this, &MainWindow::fileChanged, this, &MainWindow::showKml);
    QObject::connect(ui->BorderColor, &QPushButton::clicked, [this](){this->showColorPicker(KmlElement::LINE_COLOR);});
    QObject::connect(ui->FillColor, &QPushButton::clicked, [this](){this->showColorPicker(KmlElement::FILL_COLOR);});

    m_kmlGraphics = new KmlQmlGraphics(this);
    ui->kmlMapQtQuick->rootContext()->setContextProperty("kmlgraphics", m_kmlGraphics);
    ui->kmlMapQtQuick->setSource(QUrl("qrc:/kmlMapTest.qml"));

    ui->mapQtQuick->engine()->addImageProvider(QLatin1String("kmlimage"),  new KmlQmlImage(m_kmlGraphics->qmlImage()));
    ui->mapQtQuick->rootContext()->setContextProperty("kmlgraphics", m_kmlGraphics);
    ui->mapQtQuick->setSource(QUrl("qrc:/mapTest.qml"));
    m_accessManager = new QNetworkAccessManager();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showColorPicker(const QString& item){
    KmlDocument* doc = m_kmlGraphics->document(KML);
    for(auto element : doc->elements()){
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

void MainWindow::showKml(QString filename){
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
            QObject::connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [] (QNetworkReply::NetworkError code){
               qDebug() << "Download error " << code;
            });
        }
        //render and set as image
        ui->imageLabel->setPixmap(QPixmap::fromImage(doc->render(ui->imageLabel->size())));
        //add for QML rendering
        m_kmlGraphics->append(doc, KML);
        QObject::connect(doc, &KmlDocument::documentChanged, [this, doc](){
            ui->imageLabel->setPixmap(QPixmap::fromImage(doc->render(ui->imageLabel->size())));
        });
    }
    else{
        QMessageBox::warning(this, "Error", "Open Error");
        delete doc;
    }
}

