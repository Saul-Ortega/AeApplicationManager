#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"
#include "applicationmodel.h"
#include "applicationdelegate.h"
#include "applicationinfodialog.h"
#include "filterproxymodel.h"
#include "installerworker.h"
#include <QScrollBar>
#include <QThread>

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

    connect(this, &AvailableApplicationsWidget::changeToMenuStyle, delegate, &ApplicationDelegate::onMenuStyleClicked);
    connect(this, &AvailableApplicationsWidget::changeToMenuStyle, this, [this](){
        ui->listViewAvailable->reset();
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
void AvailableApplicationsWidget::onDownloadClicked(const QModelIndex& proxyIndex)
{
    // RECIBIMOS EL INDEX DEL PROXY Y LO PASAMOS A INDICE DEL MODELO
    QModelIndex sourceIndex = mProxyModel->mapToSource(proxyIndex);
    if (!sourceIndex.isValid()){
        return;
    }

    // COGEMOS EL ID DE LA APLICACIÓN QUE QUIERE INSTALAR
    int appId = sourceIndex.data(ApplicationModel::IdRole).toInt();

    // CREAMOS EL WORKER Y EL HILO
    QThread *thread = new QThread(this);
    installerWorker *worker = new installerWorker(appId);

    worker->moveToThread(thread);

    // CUANDO EL HILO EMITA LA SEÑAL STARTED EJECUTARA EL METODO DEL WORKER
    connect(thread, &QThread::started, worker, &installerWorker::install);

    // CADA 50MS ACTUALIZAMOS LA BARRA
    connect(worker, &installerWorker::progress, this, &AvailableApplicationsWidget::onInstallProgress);

    // CUANDO LA BARRA TERMINA ACTUALIZAMOS EL MODELO
    connect(worker, &installerWorker::finished, this, &AvailableApplicationsWidget::onInstallFinished);

    // QUITAMOS EL WORKER DEL THREAD
    connect(worker, &installerWorker::finished, thread, &QThread::quit);

    // ESPERAMOS A QUE EL THREAD TERMINE ANTES DE ELIMINAR
    connect(thread, &QThread::finished, worker, &installerWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void AvailableApplicationsWidget::onInstallProgress(int appId, int progress)
{
    // RECIBIMOS EL INDEX DEL MODELO (sourceIndex)
    QModelIndex sourceIndex = mModel->indexForAppId(appId);
    if (!sourceIndex.isValid()){
        return;
    }

    // ACTUALIZAMOS EL PROGRESO AL MODELO
    mModel->setData(sourceIndex, progress, ApplicationModel::ProgressRole);

}

void AvailableApplicationsWidget::onInstallFinished(int appId)
{
    QModelIndex sourceIndex = mModel->indexForAppId(appId);
    if (!sourceIndex.isValid()){
        return;
    }

    // PONEMOS EL PROGRESO A CERO
    mModel->setData(sourceIndex, 0, ApplicationModel::ProgressRole);

    // MARCAMOS COMO INSTALADA LA APLICACION
    mModel->setData(sourceIndex, true, ApplicationModel::IsDownloadedRole);

    // INSTALAMOS LA ULTIMA VERSION
    QList<Version> versions = mModel->data(sourceIndex, ApplicationModel::VersionsRole).value<QList<Version>>();
    if(!versions.empty()){
        versions.last().setIsInstalled(true);
    }
    mModel->setData(sourceIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
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
