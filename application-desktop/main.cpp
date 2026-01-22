#include "mainwindow.h"

#include <QApplication>
#include "Applicationdao.h"
#include <QString>

int main(int argc, char *argv[])
{
    QString d;
    ApplicationDao t =ApplicationDao(d);

    t.loadApplication();
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    return a.exec();

}
