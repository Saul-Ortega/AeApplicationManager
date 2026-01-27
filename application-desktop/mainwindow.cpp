#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "availableapplicationswidget.h"

//CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mApplicationManagerWidget(new ApplicationManagerWidget)
    , mStackedWidget(new QStackedWidget)
{
    ui->setupUi(this);

    //PONE UN TITULO A LA APLICACION Y EL LOGO
    this->setWindowTitle("AeApplicationManager");
    this->setWindowIcon(QIcon(":/assets/Logo-Aerolaser.png"));

    ApplicationModel* applicationModel = new ApplicationModel(this);
    QItemSelectionModel* applicationSelectionModel = new QItemSelectionModel(applicationModel, this);
    mApplicationManagerWidget->setApplicationModel(applicationModel);
    mApplicationManagerWidget->setApplicationSelectionModel(applicationSelectionModel);

    // VersionModel* versionModel = new VersionModel(this);
    // QItemSelectionModel* versionSelectionModel = new QItemSelectionModel(versionModel, this);
    // mApplicationManagerWidget->setVersionModel(versionModel);
    // mApplicationManagerWidget->setApplicationSelectionModel(versionSelectionModel);

    mStackedWidget->addWidget(mApplicationManagerWidget);

    setCentralWidget(mStackedWidget);
}

//DESTRUCTOR
MainWindow::~MainWindow()
{
    delete ui;
}

//SLOTS
void MainWindow::displayApplicationManager()
{
    mStackedWidget->setCurrentWidget(mApplicationManagerWidget);
}
