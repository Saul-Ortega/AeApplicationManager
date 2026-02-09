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
#include <QFile>

//CONSTRUCTOR
ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManagerWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->availableApplicationWidget, &AvailableApplicationsWidget::infoClicked, this, &ApplicationManagerWidget::onInfoClicked);
    connect(ui->installedApplicationWidget, &InstalledApplicationsWidget::infoClicked, this, &ApplicationManagerWidget::onInfoClicked);

    //RECIBE LA SEÑAL DEL SEARCH Y SE LA MANDA AL METODO DE AVAILABLE APPLICATION
    connect(ui->searchWidget, &SearchWidget::searchText, ui->availableApplicationWidget, &AvailableApplicationsWidget::onSearchText);
    connect(ui->searchWidget, &SearchWidget::searchText, ui->installedApplicationWidget, &InstalledApplicationsWidget::onSearchText);

    //RECIBE LA SEÑAL DEL ESTILO DE VISTA Y LA MANDA AL AVAILABLE E INSTALLED APPLICATION WIDGET
    connect(ui->searchWidget, &SearchWidget::changeToMenuStyle, ui->installedApplicationWidget, &InstalledApplicationsWidget::changeToMenuStyle);
    connect(ui->searchWidget, &SearchWidget::changeToMenuStyle, ui->availableApplicationWidget, &AvailableApplicationsWidget::changeToMenuStyle);

    QFile file(":/resources/styles.qss");
    if (file.open(QFile::ReadOnly)) {
        QString style = QLatin1String(file.readAll());
        qApp->setStyleSheet(style);
    }
}

//DESTRUCTOR
ApplicationManagerWidget::~ApplicationManagerWidget()
{
    delete ui;
}

//METODOS
void ApplicationManagerWidget::setApplicationModel(ApplicationModel* model)
{
    mModel = model;
    ui->availableApplicationWidget->setApplicationModel(mModel);
    ui->installedApplicationWidget->setApplicationModel(mModel);
    //TODOS: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::onInfoClicked(const QModelIndex& index)
{
    ApplicationInfoDialog *modal = new ApplicationInfoDialog(this);
    modal->setModal(true);
    modal->resize(this->width() - 100, this->height() - 100);
    modal->setApplicationModel(mModel);
    modal->loadApplication(index);
    modal->show();
}
