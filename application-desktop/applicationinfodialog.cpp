#include "applicationinfodialog.h"
#include "ui_applicationinfodialog.h"

//CONSTRUCTOR
ApplicationInfoDialog::ApplicationInfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplicationInfoDialog)
    , mApplicationModel(nullptr)
    , mIndex()
    , mVersion()
{
    ui->setupUi(this);

    //CARGA LOS DETALLES DE LA VERSIÓN CUANDO SE CAMBIA DE ÍNDICE EN EL COMBOBOX
    connect(ui->versionComboBox, &QComboBox::currentIndexChanged, this, [this] (int row) {
        QString versionName = ui->versionComboBox->itemText(row);
        QList<Version> versions = mApplicationModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();

        for ( auto version : versions ) {
            if ( version.name() == versionName ) {
                loadVersion(version);
                return;
            }
        }
    });

    //CAMBIA SI LA VERSIÓN ESTÁ INSTALADA O NO
    connect(ui->firstToolButton, &QToolButton::clicked, this, [this] () {
        QList<Version> versions = mApplicationModel->data(mIndex, ApplicationModel::VersionsRole).value<QList<Version>>();
        QString versionName = ui->versionComboBox->currentText();

        for ( int i = versions.size() - 1; i >= 0; i-- ) {
            if ( versions[i].name() == versionName ) {
                versions[i].setIsInstalled(!versions[i].isInstalled());
                mApplicationModel->setData(mIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
                loadVersion(versions[i]);
                return;
            }
        }

    });

    //CAMBIA SI LA VERSIÓN ESTÁ EN FAVORITOS O NO
    connect(ui->secondToolButton, &QToolButton::clicked, this, [this] () {
        bool favorite = mApplicationModel->data(mIndex, ApplicationModel::IsLikedRole).toBool();
        mApplicationModel->setData(mIndex, !favorite, ApplicationModel::IsLikedRole);
        loadApplication(mIndex);
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
    mApplicationModel = model;
}

void ApplicationInfoDialog::loadApplication(const QModelIndex& index)
{
    if ( !index.isValid() ) {
        return;
    }

    mIndex = index;

    this->setWindowTitle(mApplicationModel->data(index, ApplicationModel::NameRole).toString());
    this->setWindowIcon(QIcon(mApplicationModel->data(index, ApplicationModel::ImageUrlRole).toString()));

    QList<Version> versions = mApplicationModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    ui->title->setText(mApplicationModel->data(index, ApplicationModel::NameRole).toString());
    ui->description->setText(mApplicationModel->data(index, ApplicationModel::DescriptionRole).toString());
    ui->description->setWordWrap(true);

    for ( int i = versions.size() - 1; i >= 0; i-- ) {
        ui->versionComboBox->addItem(versions[i].name());
    }

    loadVersion(versions.last());

    ui->applicationLastAvailableUpdate->setText(versions.last().name());
    ui->applicationExecutableFile->setText(mApplicationModel->data(index, ApplicationModel::ExecutableFileRole).toString());

    mApplicationModel->data(index, ApplicationModel::IsLikedRole).toBool() ? ui->secondToolButton->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->secondToolButton->setIcon(QIcon(":/assets/Corazon.png"));
}

void ApplicationInfoDialog::loadVersion(const Version& version)
{
    ui->applicationSize->setText(QString::number(version.size()) + " GB");
    ui->applicationLastModificationDate->setText(version.lastModification().toString("dd/MM/yyyy"));
    version.isInstalled() ? ui->firstToolButton->setIcon(QIcon(":/assets/papelera.png")) : ui->firstToolButton->setIcon(QIcon(":/assets/Icon_Download.png"));
}
