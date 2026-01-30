#include "installedapplicationswidget.h"
#include "ui_installedapplicationswidget.h"

InstalledApplicationsWidget::InstalledApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InstalledApplicationsWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

    // // CREAR MODELO
    // mModel = new ApplicationModel(this);

    // // ASIGNAR MODELO A LA VISTA
    // ui->listViewInstalled->setModel(mModel);

    // CONFIGURACION DEL LISTVIEW
    ui->listViewInstalled->setSpacing(10);
    ui->listViewInstalled->setResizeMode(QListView::Adjust);
    ui->listViewInstalled->setFlow(QListView::LeftToRight);
    ui->listViewInstalled->setWrapping(true);
    ApplicationDelegate* delegate = new ApplicationDelegate();
    ui->listViewInstalled->setItemDelegate(delegate);


    connect(delegate, &ApplicationDelegate::isLikedButtonClicked, this, &InstalledApplicationsWidget::onLikedClicked);
    connect(delegate, &ApplicationDelegate::infoButtonClicked, this, &InstalledApplicationsWidget::infoClicked);
    connect(delegate, &ApplicationDelegate::isDownloadedButtonClicked, this, &InstalledApplicationsWidget::onDeleteClicked);


    connect(ui->installedBtn, &QPushButton::clicked, this, &InstalledApplicationsWidget::onInstalledClicked);
    connect(ui->favoriteBtn, &QPushButton::clicked, this, &InstalledApplicationsWidget::onFavoriteClicked);
}

InstalledApplicationsWidget::~InstalledApplicationsWidget()
{
    delete ui;
}

void InstalledApplicationsWidget::setApplicationModel(ApplicationModel* model)
{
    mModel = model;
    ui->listViewInstalled->setModel(mModel);
}

void InstalledApplicationsWidget::onDeleteClicked(const QModelIndex& index)
{
    //ELIMINA LA APLICACIÓN
    mModel->setData(index, false, ApplicationModel::IsDownloadedRole);

    //ELIMINA TODAS LAS VERSIONES DE DICHA APLICACIÓN
    QList<Version> versions = mModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    for ( int i = 0; i < versions.size(); i++ ) {
        versions[i].setIsInstalled(false);
    }

    mModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
}

void InstalledApplicationsWidget::onLikedClicked(const QModelIndex& index)
{
    bool isInstalled = mModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mModel->setData(index, !isInstalled, ApplicationModel::IsLikedRole);
}

void InstalledApplicationsWidget::onInstalledClicked()
{
    mShowFavorite = false;
}

void InstalledApplicationsWidget::onFavoriteClicked()
{
    mShowFavorite = true;
}
