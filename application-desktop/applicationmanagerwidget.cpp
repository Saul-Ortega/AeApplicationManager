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
#include "applicationinfodialog.h"

//CONSTRUCTOR
ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManagerWidget)
    , mApplicationModel(nullptr)
    , mVersionModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->availableApplicationWidget, &AvailableApplicationsWidget::infoClicked, this, &ApplicationManagerWidget::onInfoClicked);

    //RECIBE LA SEÑAL DEL SEARCH Y SE LA MANDA AL METODO DE AVAILABLE APPLICATION
    connect(ui->searchWidget, &SearchWidget::searchText,
            ui->availableApplicationWidget, &AvailableApplicationsWidget::onSearchText);
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
    ui->availableApplicationWidget->setApplicationModel(mApplicationModel);
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

void ApplicationManagerWidget::onInfoClicked(const QModelIndex& index)
{
    ApplicationInfoDialog *modal = new ApplicationInfoDialog(this);
    modal->setModal(true);
    modal->resize(this->width() - 100, this->height() - 100);
    modal->setApplicationModel(mApplicationModel);
    modal->loadApplication(index);
    modal->show();
}
