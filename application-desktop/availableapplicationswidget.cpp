#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"
#include "applicationmodel.h"
#include "AvailableItemWidget.h"
#include "applicationinfodialog.h"
#include "filterproxymodel.h"
#include <QTimer>

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
    //MODIFICAMOS EL ROL APPLICATION "IsDownloadRole" A TRUE CUANDO SE PULSA

    //GUARDAMOS LAS VERSIONES EN QLIST

    QList<Version> versions = mProxyModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    //MODIFICAMOS EL ROL DE VERSION "IsInstalledRole" A TRUE LA ULTIMA VERSION
    if(!versions.empty()){
        versions.last().setIsInstalled(true);
    }

    //GUARDAMOS LAS VERSIONES MODIFICADAS AL MODELO
    mProxyModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);

    mProxyModel->setData(index, true, ApplicationModel::IsDownloadedRole);

}

// CUANDO LA DESCARGA FINALIZA ELIMINAMOS EL WIDGET
// void AvailableApplicationsWidget::onDownloadFinished(int row)
// {
//     QModelIndex index = mProxyModel->index(row, 0);
//     QString name = mProxyModel->data(index, ApplicationModel::NameRole).toString();
//     qDebug() << "Descarga finalizada:" << name;

//     // RECIBIMOS EL WIDGET QUE TERMINO LA DESCARGA
//     QWidget* widget = ui->listViewAvailable->indexWidget(index);

//     if (widget) {
//         // SI EXISTE EL WIDGET LO DESCONECTAMOS
//         widget->disconnect();

//         // QUITAMOS EL WIDGET DE LA LISTA
//         ui->listViewAvailable->setIndexWidget(index, nullptr);

//         // OCULTAMOS LA FILA PARA QUE OTRO WIDGET OCUPE SU LUGAR
//         ui->listViewAvailable->setRowHidden(row, true);

//         // ELIMINAMOS EL WIDGET
//         widget->deleteLater();
//     }
// }

void AvailableApplicationsWidget::onSearchText(const QString& text) {
    if(mProxyModel){
        mProxyModel->setFilterText(text);
    }
}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
