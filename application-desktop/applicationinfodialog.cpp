#include "applicationinfodialog.h"
#include "ui_applicationinfodialog.h"

//CONSTRUCTOR
ApplicationInfoDialog::ApplicationInfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplicationInfoDialog)
    , mModel(nullptr)
    , mIndex()
    , mVersion()
{
    ui->setupUi(this);
    ui->progressBarText->setVisible(false);
    ui->progressBar->setVisible(false);

    //CARGA LOS DETALLES DE LA VERSIÓN CUANDO SE CAMBIA DE ÍNDICE EN EL COMBOBOX
    connect(ui->versionComboBox, &QComboBox::currentIndexChanged, this, [this] (int row) {
        QString versionName = ui->versionComboBox->itemText(row);
        QList<Version> versions = mModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();

        for ( auto version : versions ) {
            if ( version.name() == versionName ) {
                loadVersion(version);
                return;
            }
        }
    });

    //CAMBIA SI LA VERSIÓN ESTÁ INSTALADA O NO
    connect(ui->isInstalledBtn, &QToolButton::clicked, this, [this] () {
        // CREAMOS EL WORKER Y EL HILO
        QThread *thread = new QThread(this);
        installerWorker *worker = new installerWorker(mIndex);

        worker->moveToThread(thread);

        // CUANDO EL HILO EMITA LA SEÑAL STARTED EJECUTARA EL METODO DEL WORKER
        connect(thread, &QThread::started, worker, &installerWorker::install);

        // CADA 50MS ACTUALIZAMOS LA BARRA
        connect(worker, &installerWorker::progress, this, &ApplicationInfoDialog::onInstallProgress);

        // CUANDO LA BARRA TERMINA ACTUALIZAMOS EL MODELO
        connect(worker, &installerWorker::finished, this, &ApplicationInfoDialog::onInstallFinished);

        // QUITAMOS EL WORKER DEL THREAD
        connect(worker, &installerWorker::finished, thread, &QThread::quit);

        // ESPERAMOS A QUE EL THREAD TERMINE ANTES DE ELIMINAR
        connect(thread, &QThread::finished, worker, &installerWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

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

        QList<Version> versions = mModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();
        QString versionName = ui->versionComboBox->currentText();

        //RECORRE LAS VERSIONES DESDE LA MÁS RECIENTE HASTA LA MÁS ANTIGUA
        for ( int i = versions.size() - 1; i >= 0; i-- ) {
            //SI ENCUENTRA EL NOMBRE DE LA VERSIÓN
            if ( versions[i].name() == versionName ) {
                //ALMACENA SI ESTÁ INSTALADA O NO
                bool isInstalled = versions[i].isInstalled();
                isInstalled ? ui->progressBarText->setText("Desinstalando...") : ui->progressBarText->setText("Instalando...");
            }
        }

        //MUESTRA LA PROGRESS BAR Y EL TEXTO EN LA UI
        ui->progressBarText->setVisible(true);
        ui->progressBar->setVisible(true);
        /*DESHABILITAMOS EL QCOMBOBOX PARA QUE EL USUARIO NO PUEDA CAMBIAR DE VERSIÓN
        * HASTA QUE SE TERMINE DE INSTALAR O ELIMINAR
        */
        QWidget* versionComboBox = qobject_cast<QWidget*>(ui->versionComboBox);
        versionComboBox->setEnabled(false);
        thread->start();
    });

    //CAMBIA SI LA VERSIÓN ESTÁ EN FAVORITOS O NO
    connect(ui->isLikedBtn, &QToolButton::clicked, this, [this] () {
        bool favorite = mModel->data(mIndex, ApplicationModel::IsLikedRole).toBool();
        mModel->setData(mIndex, !favorite, ApplicationModel::IsLikedRole);
        !favorite ? ui->isLikedBtn->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->isLikedBtn->setIcon(QIcon(":/assets/Corazon.png"));
    });
}

//DESTRUCTOR
ApplicationInfoDialog::~ApplicationInfoDialog()
{
    delete ui;
}

//MÉTODOS
void ApplicationInfoDialog::setApplicationModel(ApplicationModel* model)
{
    mModel = model;
}

void ApplicationInfoDialog::loadApplication(const QModelIndex& index)
{
    if ( !index.isValid() ) {
        return;
    }

    mIndex = index;

    this->setWindowTitle(mModel->data(index, ApplicationModel::NameRole).toString());
    this->setWindowIcon(QIcon(mModel->data(index, ApplicationModel::ImageUrlRole).toString()));

    QList<Version> versions = mModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    ui->title->setText(mModel->data(index, ApplicationModel::NameRole).toString());
    ui->description->setText(mModel->data(index, ApplicationModel::DescriptionRole).toString());
    ui->description->setWordWrap(true);

    for ( int i = versions.size() - 1; i >= 0; i-- ) {
        ui->versionComboBox->addItem(versions[i].name());
    }

    loadVersion(versions.last());

    ui->applicationLastAvailableUpdate->setText(versions.last().name());
    ui->applicationExecutableFile->setText(mModel->data(index, ApplicationModel::ExecutableFileRole).toString());

    mModel->data(index, ApplicationModel::IsLikedRole).toBool() ? ui->isLikedBtn->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->isLikedBtn->setIcon(QIcon(":/assets/Corazon.png"));
}

void ApplicationInfoDialog::loadVersion(const Version& version)
{
    ui->applicationSize->setText(QString::number(version.size()) + " GB");
    ui->applicationLastModificationDate->setText(version.lastModification().toString("dd/MM/yyyy"));
    version.isInstalled() ? ui->isInstalledBtn->setIcon(QIcon(":/assets/papelera.png")) : ui->isInstalledBtn->setIcon(QIcon(":/assets/Icon_Download.png"));
}

void ApplicationInfoDialog::onInstallProgress(QModelIndex sourceIndex, int progress)
{
    if (!sourceIndex.isValid()){
        return;
    }

    // ACTUALIZAMOS EL PROGRESO AL MODELO
    mModel->setData(sourceIndex, progress, ApplicationModel::ProgressRole);
    //ACTUALIZAMOS EL VALOR DE LA PROGRESS BAR EN LA UI
    ui->isLikedBtn->setEnabled(false);
    ui->isInstalledBtn->setEnabled(false);
    ui->progressBar->setValue(progress);
}

void ApplicationInfoDialog::onInstallFinished(QModelIndex sourceIndex)
{
    if (!sourceIndex.isValid()){
        return;
    }

    QList<Version> versions = mModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();
    QString versionName = ui->versionComboBox->currentText();

    bool isApplicationDownloaded = false;

    //RECORRE LAS VERSIONES DESDE LA MÁS RECIENTE HASTA LA MÁS ANTIGUA
    for ( int i = versions.size() - 1; i >= 0; i-- ) {
        //SI ENCUENTRA EL NOMBRE DE LA VERSIÓN
        if ( versions[i].name() == versionName ) {
            //ALMACENA SI ESTÁ INSTALADA O NO
            bool isInstalled = versions[i].isInstalled();
            //ASIGNA EL VALOR CONTRARIO
            versions[i].setIsInstalled(!isInstalled);
            //CAMBIA EL ICONO DEPENDIENDO DE SI ESTÁ INSTALADO O NO
            !isInstalled ? ui->isInstalledBtn->setIcon(QIcon(":/assets/papelera.png")) : ui->isInstalledBtn->setIcon(QIcon(":/assets/Icon_Download.png"));
        }

        //COMPRUEBA SI ALGUNA VERSIÓN ESTÁ INSTALADA
        if ( versions[i].isInstalled() ) {
            isApplicationDownloaded = true;
        }
    }

    //ESCONDE LA PROGRESS BAR Y EL TEXTO
    ui->progressBarText->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->isLikedBtn->setEnabled(true);
    ui->isInstalledBtn->setEnabled(true);

    //ACTIVAMOS EL QCOMBOBOX PARA QUE SE PUEDA CAMBIAR DE VERSIÓN OTRA VEZ
    QWidget* versionComboBox = qobject_cast<QWidget*>(ui->versionComboBox);
    versionComboBox->setEnabled(true);
    //ASIGNAMOS EL PROGRESO A CERO
    mModel->setData(sourceIndex, 0, ApplicationModel::ProgressRole);
    //ASIGNAMOS LAS VERSIONES
    mModel->setData(mIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
    //ASIGNAMOS AL UPDATE EL VALOR DE SI LA ÚLTIMA VERSIÓN ESTÁ INSTALADA PARA MOSTRAR LA NOTIFICACIÓN O NO
    mModel->setData(mIndex, versions.last().isInstalled(), ApplicationModel::UpdateRole);
    //ASIGNAMOS SI LA APLICACIÓN ESTÁ DESCARGADA
    mModel->setData(mIndex, isApplicationDownloaded, ApplicationModel::IsDownloadedRole);
}
