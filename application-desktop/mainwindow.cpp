#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>

//CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mApplicationManagerWidget(new ApplicationManagerWidget)
    , mStackedWidget(new QStackedWidget)
{
    ui->setupUi(this);

    //EVITA QUE SE CIERRE LA APP CUANDO SE CIERRA LA ULTIMA VENTANA
    QApplication::setQuitOnLastWindowClosed(false);

    //PONE UN TITULO A LA APLICACION Y EL LOGO
    this->setWindowTitle("AeApplicationManager");
    this->setWindowIcon(QIcon(":/assets/Logo-Aerolaser.png"));

    ApplicationModel* applicationModel = new ApplicationModel(this);
    mApplicationManagerWidget->setApplicationModel(applicationModel);

    mStackedWidget->addWidget(mApplicationManagerWidget);

    setCentralWidget(mStackedWidget);

    //TRAYICON PARA MINIMIZAR LA APP Y EL NOMBRE EN LA AREA DE NOTIFICACION
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon (":/assets/Logo-Aerolaser.png"));
    trayIcon->setToolTip("AeApplicationManager");
    trayIcon->show();

    //CUANDO TRAYICON EMITA LA SEÑAL "activated" LLAMA AL METODO "onTrayIconActivated"
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);

    //CREAMOS EL MENU PARA PODER CERRAR LA APP
    QMenu* Menu = new QMenu(this);
    //AÑADIMOS LA ACCION DE "Salir"
    Menu->addAction("Salir", qApp, &QApplication::quit);
    //LE ASIGNAMOS EL MENU A TRAYICON
    trayIcon->setContextMenu(Menu);
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        //SI LA ACCION ES DOUBLE CLICK RESTAURA LA VENTANA Y LA MUESTRA
        showNormal();

        //LO PONE ENCIMA DE OTRAS VENTANAS
        this->raise();

        //WINDOWS LE PONE EL FOCO
        this->activateWindow();
    }
}

void MainWindow::closeEvent (QCloseEvent *event) {

    event->ignore();
    this->hide();
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
