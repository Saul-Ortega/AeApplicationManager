#include "availableitemwidget.h"
#include "ui_availableitemwidget.h"
#include <QPixmap>
#include <QDebug>

AvailableItemWidget::AvailableItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableItemWidget)
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


    // Configuración para que los botones puedan recibir clics
    ui->btn_Info->setCursor(Qt::PointingHandCursor);
    ui->btn_Favorite->setCursor(Qt::PointingHandCursor);
    ui->btn_Delete->setCursor(Qt::PointingHandCursor);

    //CREAMOS LOS CONECTORES AL CLICAR
    connect(ui->btn_Info, &QPushButton::clicked, this, &AvailableItemWidget::onInfoButtonClicked);
    connect(ui->btn_Favorite, &QPushButton::clicked, this, &AvailableItemWidget::onFavoriteButtonClicked);
    connect(ui->btn_Delete, &QPushButton::clicked, this, &AvailableItemWidget::onDeleteButtonClicked);
}


//PASAMOS EL NOMBRE DE LA APP Y EL ICONO
void AvailableItemWidget::setData(const QString& name, const QString& imagePath) {
    ui->labe_Name->setText(name);

    QPixmap pixmap(imagePath);
    if(!pixmap.isNull()){
        ui->label_Icon->setPixmap(pixmap.scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

//INDICA LA FILA EN LA QUE ESTA
void AvailableItemWidget::setRow(int row) {
    mRow = row;
}

//EMITE LA FILA AL PULSAR ALGUN BOTON
void AvailableItemWidget::onInfoButtonClicked()
{
    emit infoClicked(mRow);
}

void AvailableItemWidget::onFavoriteButtonClicked()
{
    emit favoriteClicked(mRow);
}

void AvailableItemWidget::onDeleteButtonClicked()
{
    emit deleteClicked(mRow);
}

AvailableItemWidget::~AvailableItemWidget()
{
    delete ui;
}
