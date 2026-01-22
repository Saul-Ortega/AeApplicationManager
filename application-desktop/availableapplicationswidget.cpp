#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"

#include "applicationmodel.h"

AvailableApplicationsWidget::AvailableApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableApplicationsWidget)
{
    ui->setupUi(this);

    // CREAR MODELO
    mModel = new ApplicationModel(this);

    // CREAR DELEGATE
    ApplicationDelegate *delegate = new ApplicationDelegate(this);

    // ASIGNAR DELEGATE A LIST VIEW
    ui->listViewAvailable->setItemDelegate(delegate);

    // ASIGNAR MODELO A LIST VIEW
    ui->listViewAvailable->setModel(mModel);

    // CONFIGURAR VISTA CON ICONOS
    ui->listViewAvailable->setItemDelegate(new ApplicationDelegate(this));
    ui->listViewAvailable->setModel(mModel);
    ui->listViewAvailable->setViewMode(QListView::IconMode);
    ui->listViewAvailable->setResizeMode(QListView::Adjust);
    ui->listViewAvailable->setSpacing(10);

    this->setStyleSheet("background-color: black;");    //ESTE NO FUNCIONA NO SE PORQUE
    ui->frame->setStyleSheet("background-color: white;");
    ui->listViewAvailable->setStyleSheet("background-color: white;");




}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
