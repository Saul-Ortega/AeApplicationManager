#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mApplicationManagerWidget(new ApplicationManagerWidget)
    , mStackedWidget(new QStackedWidget)
{
    ui->setupUi(this);

    //SE PONE UN TÍTULO A LA APLICACIÓN Y EL LOGO
    this->setWindowTitle("AeApplicationManager");
    this->setWindowIcon(QIcon(":/assets/Logo-Aerolaser.png"));

    mStackedWidget->addWidget(mApplicationManagerWidget);

    setCentralWidget(mStackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
