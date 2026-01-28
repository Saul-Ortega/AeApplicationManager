#include "applicationinfodialog.h"
#include "ui_applicationinfodialog.h"

//CONSTRUCTOR
ApplicationInfoDialog::ApplicationInfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplicationInfoDialog)
    , mApplicationModel(nullptr)
{
    ui->setupUi(this);
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

    this->setWindowTitle(mApplicationModel->data(index, ApplicationModel::NameRole).toString());
    this->setWindowIcon(QIcon(mApplicationModel->data(index, ApplicationModel::ImageUrlRole).toString()));

    QList<Version> versions = mApplicationModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    ui->title->setText(mApplicationModel->data(index, ApplicationModel::NameRole).toString());
    ui->description->setText(mApplicationModel->data(index, ApplicationModel::DescriptionRole).toString());
    ui->description->setWordWrap(true);
    ui->applicationSize->setText(QString::number(versions.last().size()) + " GB");;

    for ( int i = versions.size() - 1; i >= 0; i-- ) {
        ui->versionComboBox->addItem(versions[i].name());
    }

    ui->applicationLastAvailableUpdate->setText(versions.last().name());
    ui->applicationLastModificationDate->setText(versions.last().lastModification().toString("dd/MM/yyyy"));
    ui->applicationExecutableFile->setText(mApplicationModel->data(index, ApplicationModel::ExecutableFileRole).toString());

    versions.last().isInstalled() ? ui->firstToolButton->setIcon(QIcon(":/assets/papelera.png")) : ui->firstToolButton->setIcon(QIcon(":/assets/Icon_Download.png"));

    connect(ui->versionComboBox, &QComboBox::currentIndexChanged, this, [this, versions] (int row) {
        QString versionName = ui->versionComboBox->itemText(row);

        for ( auto version : versions ) {
            if ( version.name() == versionName ) {
                ui->applicationSize->setText(QString::number(version.size()) + " GB");
                ui->applicationLastModificationDate->setText(version.lastModification().toString("dd/MM/yyyy"));
                return;
            }

            version.isInstalled() ? ui->firstToolButton->setIcon(QIcon(":/assets/papelera.png")) : ui->firstToolButton->setIcon(QIcon(":/assets/Icon_Download.png"));
        }

    });

    mApplicationModel->data(index, ApplicationModel::IsLikedRole).toBool() ? ui->secondToolButton->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->secondToolButton->setIcon(QIcon(":/assets/Corazon.png"));
}
