#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"
#include "applicationmodel.h"
#include "applicationdelegate.h"
#include "applicationinfodialog.h"
#include "filterproxymodel.h"
#include <QScrollBar>

AvailableApplicationsWidget::AvailableApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableApplicationsWidget)
    , mModel(nullptr)
    , mProxyModel(nullptr)
{
    ui->setupUi(this);

    // CONFIGURACION DEL LISTVIEW
    ui->listViewAvailable->setSpacing(10);
    ui->listViewAvailable->setResizeMode(QListView::Adjust);
    ui->listViewAvailable->setFlow(QListView::LeftToRight);
    ui->listViewAvailable->setWrapping(true);
    ApplicationDelegate* delegate = new ApplicationDelegate();
    ui->listViewAvailable->setItemDelegate(delegate);
    ui->listViewAvailable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listViewAvailable->verticalScrollBar()->setSingleStep(10);

    //PERMITE SEGUIR EL MOVIMIENTO DEL CURSOR PARA HACER EL EFECTO HOVER EN EL DELEGATE
    ui->listViewAvailable->setMouseTracking(true);

    //ENVÍA LA SEÑAL DE SI EL USUARIO QUIERE LA VISTA DE TIPO GRID O MENU
    connect(this, &AvailableApplicationsWidget::changeToMenuStyle, delegate, [this, delegate] (const bool& isMenuStyle) {
        ui->listViewAvailable->reset();
        emit delegate->onMenuStyleClicked(isMenuStyle);
    });

    connect(delegate, &ApplicationDelegate::progressUpdated, [this]() {
        ui->listViewAvailable->viewport()->update();
    });

    connect(delegate, &ApplicationDelegate::isLikedButtonClicked, this, &AvailableApplicationsWidget::onLikedClicked);
    connect(delegate, &ApplicationDelegate::infoButtonClicked, this, [this] (const QModelIndex& index) {
        QModelIndex sourceIndex = mProxyModel->mapToSource(index);
        emit infoClicked(sourceIndex);
    });
    connect(delegate, &ApplicationDelegate::isDownloadedButtonClicked, this, &AvailableApplicationsWidget::onDownloadClicked);

    connect(ui->btn_Disponibles, &QPushButton::clicked, this, &AvailableApplicationsWidget::onAvailableClicked);
    connect(ui->btn_Deseados, &QPushButton::clicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
}


void AvailableApplicationsWidget::setApplicationModel(ApplicationModel* model)
{
    mModel = model;
    FilterProxyModel* proxyModel = new FilterProxyModel(this);
    proxyModel->setSourceModel(mModel);
    mProxyModel = proxyModel;
    mProxyModel->setShowInstalled(false);
    ui->listViewAvailable->setModel(mProxyModel);
}

// === BUTTONS ===

// CAMBIAMOS EL ROL DE FAVORITOS Y ACTUALIZAMOS EL CORAZON
void AvailableApplicationsWidget::onLikedClicked(const QModelIndex& index)
{
    bool isLiked = mProxyModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mProxyModel->setData(index, !isLiked, ApplicationModel::IsLikedRole);
}

//ACTIVAMOS EL FILTRO DE TODOS LOS DISPONIBLES
void AvailableApplicationsWidget::onAvailableClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Disponibles->setStyleSheet(R"(
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
    ui->btn_Deseados->setStyleSheet(R"(
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
        mProxyModel->setShowOnlyFavorites(false);
    }
}

// ACTIVAMOS EL FILTRO DE SOLO LOS DESEADOS
void AvailableApplicationsWidget::onFavoriteClicked(){

    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Disponibles->setStyleSheet(R"(
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
    ui->btn_Deseados->setStyleSheet(R"(
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
    if(mProxyModel){
        mProxyModel->setShowOnlyFavorites(true);
    }
}

// MODIFICAMOS EL ROL DE LA DESCARGA DE LA APP
void AvailableApplicationsWidget::onDownloadClicked(const QModelIndex& index)
{
    //GUARDAMOS LAS VERSIONES EN QLIST

    QList<Version> versions = mProxyModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    //MODIFICAMOS EL ROL DE VERSION "IsInstalledRole" A TRUE LA ULTIMA VERSION
    if(!versions.empty()){
        versions.last().setIsInstalled(true);
    }

    //GUARDAMOS LAS VERSIONES MODIFICADAS AL MODELO
    mProxyModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);

    //MODIFICAMOS EL ROL APPLICATION "IsDownloadRole" A TRUE CUANDO SE PULSA
    mProxyModel->setData(index, true, ApplicationModel::IsDownloadedRole);

}

void AvailableApplicationsWidget::onSearchText(const QString& text) {
    if(mProxyModel){
        mProxyModel->setFilterText(text);
    }
}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
