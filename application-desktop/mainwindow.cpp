#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "applicationinfodelegate.h"

//CONSTRUCTOR
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mApplicationManagerWidget(new ApplicationManagerWidget)
    , mStackedWidget(new QStackedWidget)
{
    ui->setupUi(this);

    //SE PONE UN TÍTULO A LA APLICACIÓN Y EL LOGO
    this->setWindowTitle("AeApplicationManager");
    this->setWindowIcon(QIcon(":/assets/Logo-Aerolaser.png"));

    ApplicationModel* applicationModel = new ApplicationModel(this);
    QItemSelectionModel* applicationSelectionModel = new QItemSelectionModel(applicationModel, this);
    mApplicationManagerWidget->setApplicationModel(applicationModel);
    mApplicationManagerWidget->setApplicationSelectionModel(applicationSelectionModel);

    // VersionModel* versionModel = new VersionModel(this);
    // QItemSelectionModel* versionSelectionModel = new QItemSelectionModel(versionModel, this);
    // mApplicationManagerWidget->setVersionModel(versionModel);
    // mApplicationManagerWidget->setApplicationSelectionModel(versionSelectionModel);

    mStackedWidget->addWidget(mApplicationManagerWidget);

    setCentralWidget(mStackedWidget);
}

//DESTRUCTOR
MainWindow::~MainWindow()
{
    delete ui;
}

//SLOTS
void MainWindow::displayApplicationManager()
{
    mStackedWidget->setCurrentWidget(mApplicationManagerWidget);
}

void MainWindow::displayApplicationInfoDelegate()
{
    // ApplicationInfoDelegate* delegate = new ApplicationInfoDelegate(mApplicationManagerWidget);

    // QPainter* painter = new QPainter();
    // QStyleOptionViewItem* option = new QStyleOptionViewItem();
    // QModelIndex* index = new QModelIndex();
    // delegate->paint(painter, *option, *index);
    QDialog* dialog = new QDialog(mApplicationManagerWidget);
    dialog->setWindowTitle("Aplicación 2");
    dialog->setFixedHeight(mApplicationManagerWidget->height() - 100);
    dialog->setFixedWidth(mApplicationManagerWidget->width() - 100);
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
    title->setText("Aplicación 2");
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
    description->setText("(Descripción)");

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
    versionComboBox->addItem("1.1.106");
    versionComboBox->addItem("1.1.105");
    versionComboBox->addItem("1.1.104");
    versionComboBox->addItem("1.1.103");
    versionComboBox->addItem("1.1.102");

    QLabel* lastAvailableUpdate = new QLabel(dialog);
    lastAvailableUpdate->setFont(*applicationFont);
    lastAvailableUpdate->setText("Última Actualización Disponible");
    lastAvailableUpdate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QLabel* applicationLastAvailableUpdate = new QLabel(dialog);
    applicationLastAvailableUpdate->setFont(*applicationFont);
    applicationLastAvailableUpdate->setText("1.1.106");

    QLabel* lastModificationDate = new QLabel(dialog);
    lastModificationDate->setFont(*applicationFont);
    lastModificationDate->setText("Fecha De Última Modificación");
    lastModificationDate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QLabel* applicationLastModificationDate = new QLabel(dialog);
    applicationLastModificationDate->setFont(*applicationFont);
    applicationLastModificationDate->setText("12/05/2025");

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
    applicationExecutableFile->setText("aplicacion2.exe");
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
