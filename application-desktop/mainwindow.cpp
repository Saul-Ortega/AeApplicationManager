#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "availableapplicationswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //SE PONE UN TITULO A LA APLICACION Y EL LOGO
    this->setWindowTitle("AeApplicationManager");
    this->setWindowIcon(QIcon(":/assets/Logo-Aerolaser.png"));

    AvailableApplicationsWidget *apps = new AvailableApplicationsWidget(this);
    setCentralWidget(apps);
}

MainWindow::~MainWindow()
{
    delete ui;
}
