#include "mainwindow.h"

#include <QApplication>
#include "Applicationdao.h"
#include <QString>
#include <QList>
#include "Application.h"

int main(int argc, char *argv[])
{
    QList <Application> qq;
    ApplicationDao t =ApplicationDao();

    t.saveAll(qq);
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    return a.exec();

}
