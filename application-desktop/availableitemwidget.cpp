#include "availableitemwidget.h"
#include "ui_availableitemwidget.h"
#include "applicationmodel.h"
#include <QPixmap>
#include <QDebug>

AvailableItemWidget::AvailableItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableItemWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

    // BORDE PARA EL LABEL DEL ICONO
    ui->label_Icon->setStyleSheet(
        "QLabel {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "   background-color: #FAFAFA;"
        "}"
        );


    // CONFIGURACION PARA QUE LOS BOTONES PUEDAN RECIBIR CLICKS
    ui->btn_Download->setCursor(Qt::PointingHandCursor);
    ui->btn_Favorite->setCursor(Qt::PointingHandCursor);
    ui->btn_Delete->setCursor(Qt::PointingHandCursor);

    //CREAMOS LOS CONECTORES AL CLICAR
    connect(ui->btn_Download, &QPushButton::clicked, this, &AvailableItemWidget::onDownloadButtonClicked);
    connect(ui->btn_Favorite, &QPushButton::clicked, this, &AvailableItemWidget::onFavoriteButtonClicked);
    connect(ui->btn_Delete, &QPushButton::clicked, this, &AvailableItemWidget::onInfoClicked);
}

//ACCESO AL MODELO
void AvailableItemWidget::setModel(ApplicationModel* model)
{
    mModel = model;
}


//PASAMOS EL NOMBRE DE LA APP Y EL ICONO
void AvailableItemWidget::setData(QModelIndex index) {
    ui->labe_Name->setText(mModel->data(index, ApplicationModel::NameRole).toString());

    QPixmap pixmap(mModel->data(index, ApplicationModel::ImageUrlRole).toString());
    if(!pixmap.isNull()){
        ui->label_Icon->setPixmap(pixmap.scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

//INDICA LA FILA EN LA QUE ESTA
void AvailableItemWidget::setRow(int row) {
    mRow = row;
}

//EMITE LA FILA AL PULSAR ALGUN BOTON
void AvailableItemWidget::onDownloadButtonClicked()
{
    emit downloadClicked(mRow);
}

void AvailableItemWidget::onFavoriteButtonClicked()
{
    emit favoriteClicked(mRow);
}

void AvailableItemWidget::onInfoClicked()
{
    emit infoClicked(mRow);
}

AvailableItemWidget::~AvailableItemWidget()
{
    delete ui;
}
