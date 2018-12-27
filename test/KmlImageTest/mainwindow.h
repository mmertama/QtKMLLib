#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void fileChanged(QString fileName);
public slots:
    void showKml(QString filename);
    void showColorPicker(const QString& item);
private:
    Ui::MainWindow *ui;
    QtKml::KmlQmlGraphics* m_kmlGraphics;
    QNetworkAccessManager* m_accessManager;
};

#endif // MAINWINDOW_H
