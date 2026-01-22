#include "applicationmanagerwidget.h"
#include "ui_applicationmanagerwidget.h"

ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManagerWidget)
{
    ui->setupUi(this);
}

ApplicationManagerWidget::~ApplicationManagerWidget()
{
    delete ui;
}
