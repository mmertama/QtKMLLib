#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <QDirIterator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto f = QSurfaceFormat::defaultFormat();
    f.setSamples(16);
    QSurfaceFormat::setDefaultFormat(f);

    MainWindow w;
    w.show();

    return a.exec();
}
