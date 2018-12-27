#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QtWebView>
#include <QDebug>
#include "qmlkml.h"

int main(int argc, char *argv[]){
    QGuiApplication app(argc, argv);
    QtWebView::initialize();

    QFile kml(":/Teknopolis.kml");
    Q_ASSERT(kml.exists());
    kml.open(QIODevice::ReadOnly | QIODevice::Text);
    QScopedPointer<QtKml::KmlDocument> document(new QtKml::KmlDocument());
    QScopedPointer<QtKml::KmlQmlGraphics> graphics(new QtKml::KmlQmlGraphics());
    document->open(kml);
    kml.close();
    graphics->append(document.data());

    //document->update(QSize(640, 640), 14, document->centerPoint());

    QQmlApplicationEngine engine;
    auto pp = new QtKml::KmlQmlImage(graphics->qmlImage());
    engine.addImageProvider(QLatin1String("kmlimage"), pp);
    engine.rootContext()->setContextProperty("kmlgraphics", graphics.data());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
