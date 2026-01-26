#include "applicationmanagerwidget.h"
#include "ui_applicationmanagerwidget.h"
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "applicationmodel.h"

//CONSTRUCTOR
ApplicationManagerWidget::ApplicationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplicationManagerWidget)
{
    ui->setupUi(this);

    connect(ui->searchWidget, &SearchWidget::displayApplicationInfo, this, [this] () {
        QModelIndex index = mApplicationModel->index(0,0);
        QItemSelection* t = new QItemSelection(index, index);
        displayApplicationInfo(*t);
    });
}

//DESTRUCTOR
ApplicationManagerWidget::~ApplicationManagerWidget()
{
    delete ui;
}

//MÉTODOS
void ApplicationManagerWidget::setApplicationModel(ApplicationModel* applicationModel)
{
    mApplicationModel = applicationModel;
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setApplicationSelectionModel(QItemSelectionModel* applicationSelectionModel)
{
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setVersionModel(VersionModel* versionModel)
{
    mVersionModel = versionModel;
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::setVersionSelectionModel(QItemSelectionModel* versionSelectionModel)
{
    //TODO: IMPLEMENTAR LOS MODELOS CUANDO TENGAMOS LA VISTA CREADA
}

void ApplicationManagerWidget::displayApplicationInfo(const QItemSelection& selected)
{
    if ( selected.isEmpty() ) {
        return;
    }

    QModelIndex index = selected.indexes().first();
    QList<Version> versions = index.model()->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(index.model()->data(index, ApplicationModel::NameRole).toString());
    dialog->setFixedHeight(this->height() - 100);
    dialog->setFixedWidth(this->width() - 100);
    dialog->setStyleSheet("*{background-color: #333333; color: white}");
    dialog->setModal(true);

    QFont* applicationFont = new QFont();
    applicationFont->setPixelSize(16);

    QVBoxLayout* verticalLayout = new QVBoxLayout(dialog);
    verticalLayout->setAlignment(Qt::AlignHCenter);

    //TÍTULO DE LA APLICACIÓN
    QLabel* title = new QLabel(dialog);
    QFont* titleFont = new QFont();
    titleFont->bold();
    titleFont->setPixelSize(50);
    title->setFont(*titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setText(index.model()->data(index, ApplicationModel::NameRole).toString());
    title->setStyleSheet(R"(
        QLabel {
            background-color: "#4fa0d8";
            border-radius: "15px";
        }
    )");

    QLabel* description = new QLabel(dialog);
    description->setFont(*applicationFont);
    description->setAlignment(Qt::AlignCenter);
    description->setFixedWidth(dialog->width());
    description->setFixedHeight(50);
    description->setText(index.model()->data(index, ApplicationModel::DescriptionRole).toString());

    QLabel* size = new QLabel(dialog);
    size->setFont(*applicationFont);
    size->setText("Tamaño");
    size->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QLabel* applicationSize = new QLabel(dialog);
    applicationSize->setFont(*applicationFont);
    applicationSize->setText("3 GB");

    QLabel* version = new QLabel(dialog);
    version->setFont(*applicationFont);
    version->setText("Versión");
    version->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QComboBox* versionComboBox = new QComboBox(dialog);
    versionComboBox->setFont(*applicationFont);
    for ( int i = versions.size() - 1; i >= 0; i-- ) {
         versionComboBox->addItem(versions[i].name());
    }

    connect(versionComboBox, &QComboBox::currentIndexChanged, this, [this] (int index) {
        qDebug() << "Ha cambiado";
    });

    QLabel* lastAvailableUpdate = new QLabel(dialog);
    lastAvailableUpdate->setFont(*applicationFont);
    lastAvailableUpdate->setText("Última Actualización Disponible");
    lastAvailableUpdate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QLabel* applicationLastAvailableUpdate = new QLabel(dialog);
    applicationLastAvailableUpdate->setFont(*applicationFont);
    applicationLastAvailableUpdate->setText(versions.last().name());

    QLabel* lastModificationDate = new QLabel(dialog);
    lastModificationDate->setFont(*applicationFont);
    lastModificationDate->setText("Fecha De Última Modificación");
    lastModificationDate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QLabel* applicationLastModificationDate = new QLabel(dialog);
    applicationLastModificationDate->setFont(*applicationFont);
    applicationLastModificationDate->setText(versions[versionComboBox->currentIndex()].lastModification().toString("dd/MM/yyyy"));

    QLabel* executableFile = new QLabel(dialog);
    executableFile->setFont(*applicationFont);
    executableFile->setText("Archivo Ejecutable");
    executableFile->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    executableFile->setStyleSheet(R"(
        QLabel {
            margin-bottom: 50px;
        }
    )");

    QLabel* applicationExecutableFile = new QLabel(dialog);
    applicationExecutableFile->setFont(*applicationFont);
    applicationExecutableFile->setText(index.model()->data(index, ApplicationModel::ExecutableFileRole).toString());
    applicationExecutableFile->setStyleSheet(R"(
        QLabel {
            margin-bottom: 50px;
        }
    )");

    QToolButton* firstToolButton = new QToolButton();
    firstToolButton->setIcon(QIcon(":/assets/papelera.png"));
    firstToolButton->setIconSize(QSize(120, 120));
    firstToolButton->setMinimumSize(QSize(120, 120));
    firstToolButton->setStyleSheet(R"(
        QToolButton {
            background-color: white;
            border-radius: 60%;
            margin-right: 25px;
        }
        QToolButton:hover {
            background-color: "#4fa0d8";
        }
    )");

    QToolButton* secondToolButton = new QToolButton();
    secondToolButton->setIcon(QIcon(":/assets/Corazon.png"));
    secondToolButton->setIconSize(QSize(120, 120));
    secondToolButton->setMinimumSize(QSize(120, 120));
    secondToolButton->setStyleSheet(R"(
        QToolButton {
            background-color: white;
            border-radius: 60%;
            margin-left: 25px;
        }
        QToolButton:hover {
            background-color: "#4fa0d8";
        }
    )");

    verticalLayout->addWidget(title);
    verticalLayout->addWidget(description);

    QHBoxLayout* sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(size);
    sizeLayout->addWidget(applicationSize);
    sizeLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(sizeLayout);

    QHBoxLayout* versionLayout = new QHBoxLayout();
    versionLayout->addWidget(version);
    versionLayout->addWidget(versionComboBox);
    versionLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(versionLayout);

    QHBoxLayout* lastAvailableUpdateLayout = new QHBoxLayout();
    lastAvailableUpdateLayout->addWidget(lastAvailableUpdate);
    lastAvailableUpdateLayout->addWidget(applicationLastAvailableUpdate);
    lastAvailableUpdateLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(lastAvailableUpdateLayout);

    QHBoxLayout* lastModificationDateLayout = new QHBoxLayout();
    lastModificationDateLayout->addWidget(lastModificationDate);
    lastModificationDateLayout->addWidget(applicationLastModificationDate);
    lastModificationDateLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(lastModificationDateLayout);

    QHBoxLayout* executableFileLayout = new QHBoxLayout();
    executableFileLayout->addWidget(executableFile);
    executableFileLayout->addWidget(applicationExecutableFile);
    executableFileLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(executableFileLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(firstToolButton);
    buttonsLayout->addWidget(secondToolButton);
    buttonsLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addLayout(buttonsLayout);

    dialog->open();
}
