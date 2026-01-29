#include "availableitemwidget.h"
#include "ui_availableitemwidget.h"
#include "applicationmodel.h"
#include <QPixmap>
#include <QDebug>
#include <QTimer>

AvailableItemWidget::AvailableItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableItemWidget)
    , mModel(nullptr)
    , mIndex()
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

    //BORDE PARA LOS BOTONES
    ui->btn_Download->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 6px;"
        "   padding: 4px 8px;"
        "}"
        "QPushButton:hover {"
        "   border-color: #888888;"
        "}"
        );
    ui->btn_Favorite->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 6px;"
        "   padding: 4px 8px;"
        "}"
        "QPushButton:hover {"
        "   border-color: #888888;"
        "}"
        );
    ui->btn_Info->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 6px;"
        "   padding: 4px 8px;"
        "}"
        "QPushButton:hover {"
        "   border-color: #888888;"
        "}"
        );

    // CONFIGURACION PARA QUE LOS BOTONES PUEDAN RECIBIR CLICKS
    ui->btn_Download->setCursor(Qt::PointingHandCursor);
    ui->btn_Favorite->setCursor(Qt::PointingHandCursor);
    ui->btn_Info->setCursor(Qt::PointingHandCursor);

    //CREAMOS LOS CONECTORES AL CLICAR
    connect(ui->btn_Download, &QPushButton::clicked, this, &AvailableItemWidget::onDownloadButtonClicked);
    connect(ui->btn_Favorite, &QPushButton::clicked, this, &AvailableItemWidget::onFavoriteButtonClicked);
    connect(ui->btn_Info, &QPushButton::clicked, this, [this] () {
        emit infoClicked(mIndex);
    });

    //CREAMOS UN QTIMER PARA LA PROGRESSBAR
    mProgressTimer = new QTimer(this);

    //CUANDO EL TIMER HAGA TIMEOUT SE EJECUTARA EL METODO UPDATEPROGRESS
    connect(mProgressTimer, &QTimer::timeout, this, &AvailableItemWidget::updateProgress);

    //LA PROGRESSBAR NO SE VE Y EMPIEZA EN CERO
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
}

//ACCESO AL MODELO
void AvailableItemWidget::setModel(ApplicationModel* model)
{
    mModel = model;
}

//PASAMOS EL NOMBRE DE LA APP Y EL ICONO
void AvailableItemWidget::setData(QModelIndex index) {

    mIndex = index;

    //INSERTAMOS EL NOMBRE
    ui->labe_Name->setText(mModel->data(index, ApplicationModel::NameRole).toString());

    //INSERTAMOS LA IMAGEN
    QPixmap pixmap(mModel->data(index, ApplicationModel::ImageUrlRole).toString());
    if(!pixmap.isNull()){
        ui->label_Icon->setPixmap(pixmap.scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    //SI ESTA EN FAVORITOS PONE UN CORAZON U OTRO
    bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

    isLiked ? ui->btn_Favorite->setIcon(QIcon(":/assets/CorazonSeleccionado.png")) : ui->btn_Favorite->setIcon(QIcon(":/assets/Corazon.png"));

}

//INDICA LA FILA EN LA QUE ESTA
void AvailableItemWidget::setRow(int row) {
    mRow = row;
}


//=== BUTTONS ===

void AvailableItemWidget::onDownloadButtonClicked()
{
    emit downloadClicked(mRow);
    startProgress();
}

void AvailableItemWidget::onFavoriteButtonClicked()
{
    emit favoriteClicked(mRow);
}


//=== PROGRESSBAR ===

void AvailableItemWidget::startProgress()
{
    //HACEMOS VISIBLE LA PROGRESSBRA, PONEMOS Y MOSTRAMOS EL VALOR EN CERO
    mProgressValue = 0;
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);

    // SI EL PROGRESSTIMER ESTABA ACTIVO DE ANTES LO PARAMOS
    if (mProgressTimer->isActive()){
        mProgressTimer->stop();
    }

    // CADA 5ms HACE TIMEOUT Y LLAMARA A UPDATEPROGRESS CON CONNECT
    mProgressTimer->start(40);
}

void AvailableItemWidget::updateProgress()
{
    //EL VALOR VA INCREMENTANDO Y LO APLICA A LA VISTA
    mProgressValue++;
    ui->progressBar->setValue(mProgressValue);

    //SI EL VALOR LLEGA A 100 SE PARA Y DESAPARECE
    if (mProgressValue >= 100) {

        //DETIENE EL TIMMER
        mProgressTimer->stop();

        //CUANDO PASEN 300ms LLAMARA AL METODO onDownloadComplete
        QTimer::singleShot(300, this, &AvailableItemWidget::onDownloadComplete);
    }
}

// DESACTIVARA LA VISTA DE LA PROGRESSBAR Y MANDARA UNA SEÑAL
void AvailableItemWidget::onDownloadComplete()
{
    ui->progressBar->setVisible(false);

    // EMITE LA SEÑAL DE QUE LA DESCARGA TERMINO
    emit downloadFinished(mRow);
}


AvailableItemWidget::~AvailableItemWidget()
{
    delete ui;
}
