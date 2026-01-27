#include "applicationmanagerwidget.h"
#include "ui_applicationmanagerwidget.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "applicationmodel.h"

//CONSTRUCTOR
ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManagerWidget)
{
    ui->setupUi(this);
}

//DESTRUCTOR
ApplicationManagerWidget::~ApplicationManagerWidget()
{
    delete ui;
}

//MÉTODOS
void ApplicationManagerWidget::setApplicationModel(ApplicationModel* applicationModel)
{
    mApplicationModel = applicationModel;
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setApplicationSelectionModel(QItemSelectionModel* applicationSelectionModel)
{
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setVersionModel(VersionModel* versionModel)
{
    mVersionModel = versionModel;
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setVersionSelectionModel(QItemSelectionModel* versionSelectionModel)
{
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}
