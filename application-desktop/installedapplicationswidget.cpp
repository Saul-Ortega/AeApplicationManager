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
    ui->listViewInstalled->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //ENVÍA LA SEÑAL DE SI EL USUARIO QUIERE LA VISTA DE TIPO GRID O MENU
    connect(this, &InstalledApplicationsWidget::changeToMenuStyle, delegate, &ApplicationDelegate::onMenuStyleClicked);
    connect(this, &InstalledApplicationsWidget::changeToMenuStyle, this, [this](){
        ui->listViewInstalled->reset();
    });

    connect(delegate, &ApplicationDelegate::progressUpdated, [this]() {
        ui->listViewInstalled->viewport()->update();
    });

    connect(delegate, &ApplicationDelegate::isLikedButtonClicked, this, &InstalledApplicationsWidget::onLikedClicked);
    connect(delegate, &ApplicationDelegate::infoButtonClicked, this, [this] (const QModelIndex &index) {
        QModelIndex sourceIndex = mProxyModel->mapToSource(index);
        emit infoClicked(sourceIndex);
    });
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

    //ELIMINA TODAS LAS VERSIONES DE DICHA APLICACIÓN
    QList<Version> versions = mProxyModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    for ( int i = 0; i < versions.size(); i++ ) {
        versions[i].setIsInstalled(false);
    }

    //ELIMINA LA APLICACIÓN
    QModelIndex sourceIndex = mProxyModel->mapToSource(index);
    mModel->setData(sourceIndex, false, ApplicationModel::IsDownloadedRole);

    mModel->setData(sourceIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
}

void InstalledApplicationsWidget::onLikedClicked(const QModelIndex& index)
{
    bool isLiked = mProxyModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mProxyModel->setData(index, !isLiked, ApplicationModel::IsLikedRole);
}

void InstalledApplicationsWidget::onInstalledClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->installedBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #4fa0d8;
        border: 2px solid #4fa0d8;
        border-radius: 5px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #4fa0d8;
    }
    )");
    ui->favoriteBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #91c3e8;
        border: 2px solid #4fa0d8;
        border-radius: 5px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #4fa0d8;
    }
    )");

    //SI PULSA EL BOTON NO SOLO MOSTRARA LOS FAVORITOS
    if(mProxyModel){
        mProxyModel->setShowInstalled(true);
        mProxyModel->setShowOnlyFavorites(false);
    }
}

void InstalledApplicationsWidget::onFavoriteClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->installedBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #91c3e8;
        border: 2px solid #4fa0d8;
        border-radius: 5px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #4fa0d8;
    }
    )");
    ui->favoriteBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #4fa0d8;
        border: 2px solid #4fa0d8;
        border-radius: 5px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #4fa0d8;
    }
    )");

    //SI PULSA EL BOTON SOLO MOSTRARA LOS FAVORITOS
    if ( mProxyModel ) {
        mProxyModel->setShowInstalled(true);
        mProxyModel->setShowOnlyFavorites(true);
    }
}

void InstalledApplicationsWidget::onSearchText(const QString& text)
{
    if ( mProxyModel ) {
        mProxyModel->setFilterText(text);
    }
}
