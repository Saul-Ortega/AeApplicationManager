#include "applicationinfodialog.h"
#include "ui_applicationinfodialog.h"

//CONSTRUCTOR
ApplicationInfoDialog::ApplicationInfoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplicationInfoDialog)
    , mApplicationModel(nullptr)
    , mApplicationSelectionModel(nullptr)
    , mVersionModel(nullptr)
    , mVersionSelectionModel(nullptr)
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

    connect(mApplicationModel, &QAbstractItemModel::dataChanged, [this] (const QModelIndex &topLeft, const QModelIndex &bottomRight) {
        if ( topLeft == mApplicationSelectionModel->currentIndex() ) {
            loadApplication(mApplicationSelectionModel->selection());
        }
    });
}

void ApplicationInfoDialog::setApplicationSelectionModel(QItemSelectionModel* selectionModel)
{
    mApplicationSelectionModel = selectionModel;

    connect(mApplicationSelectionModel, &QItemSelectionModel::selectionChanged, [this] (const QItemSelection &selected) {
        if ( selected.isEmpty() )  {
            return;
        }

        loadApplication(selected);
    });
}

void ApplicationInfoDialog::setVersionModel(VersionModel* model)
{
    mVersionModel = model;

    connect(mVersionModel, &QAbstractItemModel::dataChanged, [this] (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles = QList<int>()) {
        if ( topLeft == mVersionSelectionModel->currentIndex() ) {
            loadVersion(mVersionSelectionModel->selection());
        }
    });
}

void ApplicationInfoDialog::setVersionSelectionModel(QItemSelectionModel* selectionModel)
{
    mVersionSelectionModel = selectionModel;

    connect(mVersionSelectionModel, &QItemSelectionModel::selectionChanged, [this] (const QItemSelection &selected) {
        if ( selected.isEmpty() ) {
            return;
        }
        loadVersion(selected);
    });
}

void ApplicationInfoDialog::loadVersion(const QItemSelection& selected)
{
    if ( selected.isEmpty() ) {
        return;
    }

    QModelIndex index = selected.indexes().first();

    ui->applicationSize->setText(mVersionModel->data(index, VersionModel::SizeRole).toString());
    ui->lastModificationDate->setText(mVersionModel->data(index, VersionModel::LastModificationRole).value<QDate>().toString("dd/MM/yyyy"));
}

void ApplicationInfoDialog::loadApplication(const QItemSelection& selected)
{
    if ( selected.isEmpty() ) {
        return;
    }

    QModelIndex index = selected.indexes().first();
    QList<Version> versions = mApplicationModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    ui->title->setText(mApplicationModel->data(index, ApplicationModel::NameRole).toString());
    ui->description->setText(mApplicationModel->data(index, ApplicationModel::DescriptionRole).toString());

    for ( auto version : versions ) {
        ui->versionComboBox->addItem(version.name());
    }

    ui->applicationLastAvailableUpdate->setText(versions.last().name());
    ui->applicationExecutableFile->setText(mApplicationModel->data(index, ApplicationModel::ExecutableFileRole).toString());

    //TODO: IMPLEMENTAR EL MOSTRAR UN BOTÓN U OTRO
}
