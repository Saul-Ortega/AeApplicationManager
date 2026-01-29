#include "installeditemwidget.h"
#include "ui_installeditemwidget.h"

InstalledItemWidget::InstalledItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InstalledItemWidget)
    , mModel(nullptr)
    , mIndex()
{
    ui->setupUi(this);


    connect(ui->likedBtn, &QPushButton::clicked, this, [this] () {
        emit likedClicked(mRow);
    });

    connect(ui->infoBtn, &QPushButton::clicked, this, [this] () {
        emit infoClicked(mIndex);
    });

    connect(ui->deleteBtn, &QPushButton::clicked, this, [this] () {
        emit deleteClicked(mRow);
    });
}

InstalledItemWidget::~InstalledItemWidget()
{
    delete ui;
}

void InstalledItemWidget::setModel(ApplicationModel* model)
{
    mModel = model;
}

void InstalledItemWidget::setData(QModelIndex index)
{
    mIndex = index;

    //INSERTAMOS EL NOMBRE
    ui->labelName->setText(mModel->data(index, ApplicationModel::NameRole).toString());

    //INSERTAMOS LA IMAGEN
    QPixmap pixmap(mModel->data(index, ApplicationModel::ImageUrlRole).toString());
    if(!pixmap.isNull()){
        ui->labelIcon->setPixmap(pixmap.scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    //SI ESTA EN FAVORITOS PONE UN CORAZON U OTRO
    bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

    isLiked ? ui->likedBtn->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->likedBtn->setIcon(QIcon(":/assets/Corazon.png"));
}

void InstalledItemWidget::setRow(int row)
{
    mRow = row;
}
