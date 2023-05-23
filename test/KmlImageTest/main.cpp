#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <QDirIterator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDirIterator it(":", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << "RAs" << it.next();
    }

    auto f = QSurfaceFormat::defaultFormat();
    f.setSamples(16);
    QSurfaceFormat::setDefaultFormat(f);

    MainWindow w;
    w.show();

    return a.exec();
}
