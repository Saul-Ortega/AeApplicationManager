#include "applicationmanagerwidget.h"
#include "ui_applicationmanagerwidget.h"

ApplicationManager::ApplicationManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManager)
{
    ui->setupUi(this);
}

ApplicationManager::~ApplicationManager()
{
    delete ui;
}
