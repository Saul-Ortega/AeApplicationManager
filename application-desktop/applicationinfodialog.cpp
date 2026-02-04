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
        QList<Version> versions = mModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();
        QString versionName = ui->versionComboBox->currentText();

        bool isApplicationDownloaded = false;

        for ( int i = versions.size() - 1; i >= 0; i-- ) {
            if ( versions[i].name() == versionName ) {
                bool isInstalled = versions[i].isInstalled();
                versions[i].setIsInstalled(!isInstalled);
                mModel->setData(mIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
                !isInstalled ? ui->isInstalledBtn->setIcon(QIcon(":/assets/papelera.png")) : ui->isInstalledBtn->setIcon(QIcon(":/assets/Icon_Download.png"));
            }

            if ( versions[i].isInstalled() ) {
                isApplicationDownloaded = true;
            }
        }

        mModel->setData(mIndex, versions.last().isInstalled(), ApplicationModel::UpdateRole);
        mModel->setData(mIndex, isApplicationDownloaded, ApplicationModel::IsDownloadedRole);
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
