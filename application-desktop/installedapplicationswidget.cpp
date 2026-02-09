#include "installedapplicationswidget.h"
#include "ui_installedapplicationswidget.h"
#include "installerworker.h"
#include <QScrollBar>
#include <QThread>

InstalledApplicationsWidget::InstalledApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InstalledApplicationsWidget)
    , mModel(nullptr)
    , mProxyModel(nullptr)
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
    ui->listViewInstalled->verticalScrollBar()->setSingleStep(10);

    //PERMITE SEGUIR EL MOVIMIENTO DEL CURSOR PARA HACER EL EFECTO HOVER EN EL DELEGATE
    ui->listViewInstalled->setMouseTracking(true);

    //ENVÍA LA SEÑAL DE SI EL USUARIO QUIERE LA VISTA DE TIPO GRID O MENU
    connect(this, &InstalledApplicationsWidget::changeToMenuStyle, delegate, [this, delegate] (const bool& isMenuStyle) {
        ui->listViewInstalled->reset();
        emit delegate->onMenuStyleClicked(isMenuStyle);
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
    //PASAMOS DEL PROXY INDEX AL MODEL INDEX
    QModelIndex sourceIndex = mProxyModel->mapToSource(index);

    if (!sourceIndex.isValid()){
        return;
    }

    //ALMACENA EL VALOR DEL PROGRESS BAR
    int progressbar = mProxyModel->data(index, ApplicationModel::ProgressRole).toInt();

    //COMPRUEBA SI EL HILO NO SE HA LANZADO YA
    if ( progressbar == 0 ) {
        // CREAMOS EL WORKER Y EL HILO
        QThread *thread = new QThread(this);
        installerWorker *worker = new installerWorker(sourceIndex);

        worker->moveToThread(thread);

        // CUANDO EL HILO EMITA LA SEÑAL STARTED EJECUTARA EL METODO DEL WORKER
        connect (thread, &QThread::started, worker, &installerWorker::install);

        // CADA 50MS ACTUALIZAMOS LA PROGRESSBAR
        connect (worker, &installerWorker::progress, this, &InstalledApplicationsWidget::onUninstallProgress);

        // CUANDO LA BARRA TERMINA ACTUALIZAMOS EL MODELO
        connect (worker, &installerWorker::finished, this, &InstalledApplicationsWidget::onUnistallFinished);

        // QUITAMOS EL WORKER DEL THREAD
        connect (worker, &installerWorker::finished, thread, &QThread::quit);

        // ESPERAMOS A QUE EL THREAD TERMINE ANTES DE ELIMINAR
        connect (thread, &QThread::finished, worker, &installerWorker::deleteLater);
        connect (thread, &QThread::finished, thread, &QThread::deleteLater);

        /*
        * CUANDO LA APLICACIÓN EMITE LA SEÑAL DE QUE SE VA A DEJAR DE EJECUTAR,
        * ELIMINA EL HILO, ESPERA A QUE SE ELIMINE Y LUEGO YA DEJA DE EJECUTAR LA
        * APLICACIÓN
        */
        connect(qApp, &QApplication::aboutToQuit, thread, [this, thread] () {
            thread->quit();
            thread->wait();
            emit deleteLater();
        });

        thread->start();
    }
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
        color: black;
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
        color: black;
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
        color: black;
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
        color: black;
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

void InstalledApplicationsWidget::onUninstallProgress(QModelIndex sourceIndex, int progress)
{
    if (!sourceIndex.isValid()){
        return;
    }

    //VAMOS ACTUALIZANDO EL PROGRESO EN EL MODELO
    mModel->setData(sourceIndex, progress, ApplicationModel::ProgressRole);
}

void InstalledApplicationsWidget::onUnistallFinished(QModelIndex sourceIndex)
{
    // PONEMOS EL PROGRESO A CERO
    mModel->setData(sourceIndex, 0, ApplicationModel::ProgressRole);

    //ELIMINA TODAS LAS VERSIONES DE DICHA APLICACIÓN
    QList<Version> versions = mModel->data(sourceIndex, ApplicationModel::VersionsRole).value<QList<Version>>();

    for ( int i = 0; i < versions.size(); i++ ) {
        versions[i].setIsInstalled(false);
    }

    // ELIMINA LA APLICACIÓN
    mModel->setData(sourceIndex, false, ApplicationModel::IsDownloadedRole);
    mModel->setData(sourceIndex, false, ApplicationModel::UpdateRole);
    mModel->setData(sourceIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
}


void InstalledApplicationsWidget::onSearchText(const QString& text)
{
    if ( mProxyModel ) {
        mProxyModel->setFilterText(text);
    }
}
