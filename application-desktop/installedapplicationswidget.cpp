#include "installedapplicationswidget.h"
#include "ui_installedapplicationswidget.h"

InstalledApplicationsWidget::InstalledApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InstalledApplicationsWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

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
    FilterProxyModel* proxyModel = new FilterProxyModel(this);
    proxyModel->setSourceModel(mModel);
    mProxyModel = proxyModel;
    mProxyModel->setShowInstalled(true);
    ui->listViewInstalled->setModel(mProxyModel);
}

void InstalledApplicationsWidget::onDeleteClicked(const QModelIndex& index)
{
    //ELIMINA LA APLICACIÓN
    mProxyModel->setData(index, false, ApplicationModel::IsDownloadedRole);

    //ELIMINA TODAS LAS VERSIONES DE DICHA APLICACIÓN
    QList<Version> versions = mProxyModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    for ( int i = 0; i < versions.size(); i++ ) {
        versions[i].setIsInstalled(false);
    }

    mProxyModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
}

void InstalledApplicationsWidget::onLikedClicked(const QModelIndex& index)
{
    bool isLiked = mProxyModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mProxyModel->setData(index, !isLiked, ApplicationModel::IsLikedRole);
}

void InstalledApplicationsWidget::onInstalledClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->installedBtn->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->favoriteBtn->setStyleSheet("");

    //SI PULSA EL BOTON NO SOLO MOSTRARA LOS FAVORITOS
    if(mProxyModel){
        mProxyModel->setShowInstalled(true);
        mProxyModel->setShowOnlyFavorites(false);
    }
}

void InstalledApplicationsWidget::onFavoriteClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->favoriteBtn->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->installedBtn->setStyleSheet("");

    //SI PULSA EL BOTON SOLO MOSTRARA LOS FAVORITOS
    if(mProxyModel){
        mProxyModel->setShowInstalled(true);
        mProxyModel->setShowOnlyFavorites(true);
    }
}

void InstalledApplicationsWidget::onSearchText(const QString& text)
{
    if(mProxyModel){
        mProxyModel->setFilterText(text);
    }
}
