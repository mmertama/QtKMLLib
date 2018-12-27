#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGeoCoordinate>

namespace Ui {
class MainWindow;
}

namespace QtKml {
class KmlQmlGraphics;
}

class QNetworkAccessManager;

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(); 
signals:
    void fileChanged(const QString& fileName);
public slots:
    void showKml(const QString& filename);
    void showJson(const QString& filename, const QString& id);
    void showColorPicker(const QString& item);
    void startLineDraw();
private slots:
    void mapChanged();
private:
    Ui::MainWindow *ui;
    QtKml::KmlQmlGraphics* m_kmlGraphics;
    QNetworkAccessManager* m_accessManager;
    int m_index = 0;
    QTimer m_timer;
    QPolygonF m_field;
    QHash<QString, QList<QPair<QGeoCoordinate, QPointF>>> m_paths;
    QString m_currentDoc;
    QString m_kmlId;
};

#endif // MAINWINDOW_H
