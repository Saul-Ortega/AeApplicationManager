#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"
#include "applicationmodel.h"
#include "AvailableItemWidget.h"

AvailableApplicationsWidget::AvailableApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableApplicationsWidget)
{
    ui->setupUi(this);

    // CREAR MODELO
    mModel = new ApplicationModel(this);

    // ASIGNAR MODELO A LA VISTA
    ui->listViewAvailable->setModel(mModel);

    // CONFIGURACION DEL LISTVIEW
    ui->listViewAvailable->setViewMode(QListView::ListMode);
    ui->listViewAvailable->setWrapping(true);
    ui->listViewAvailable->setResizeMode(QListView::Adjust);
    ui->listViewAvailable->setUniformItemSizes(true);
    ui->listViewAvailable->setSpacing(10);
    ui->listViewAvailable->setFlow(QListView::LeftToRight);
    ui->listViewAvailable->setGridSize(QSize(200, 170));

    //ACTIVAR EL FUNCIONAMIENTO DE LOS BOTONES EN LA LISTA
    ui->listViewAvailable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listViewAvailable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listViewAvailable->setFocusPolicy(Qt::NoFocus);

    // INSERTAMOS EL WIDGET POR CADA APLICACION
    for (int i = 0; i < mModel->rowCount(); i++) {
        QModelIndex index = mModel->index(i, 0);

            // CREAMOS EL WIDGET
            AvailableItemWidget *widget = new AvailableItemWidget();

            // OBTENEMOS EL NOMBRE Y LA IMAGEN
            widget->setModel(mModel);
            widget->setData(index);

            // ASIGNAMOS LA FILA AL WIDGET
            widget->setRow(i);

            // INSERTAMOS EL WIDGET EN LA VISTA
            ui->listViewAvailable->setIndexWidget(index, widget);

            // CONNECTS
            connect(widget, &AvailableItemWidget::downloadClicked, this, &AvailableApplicationsWidget::onDownloadButtonClicked);
            connect(widget, &AvailableItemWidget::favoriteClicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
            connect(widget, &AvailableItemWidget::infoClicked, this, &AvailableApplicationsWidget::infoClicked);
    }

    // FONDO EN BLANCO
    ui->frame->setStyleSheet("background-color: #FFFFFF;");
    ui->listViewAvailable->setStyleSheet("background-color: #FFFFFF; border: none;");
}

void AvailableApplicationsWidget::onDownloadButtonClicked(int row)
{
    //RECIBE EL INDEX DE LA FILA Y EL ROL
    QModelIndex index = mModel->index(row,0);

    //MODIFICAMOS EL ROL "IsDownloadRole" A LO CONTRARIO CUANDO SE PULSA
    mModel->setData(index, true, ApplicationModel::IsDownloadedRole);


    QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    qDebug() << "Boton Download clicado por:" << name;
}

void AvailableApplicationsWidget::onFavoriteClicked(int row)
{
    //RECIBE EL INDEX DE LA FILA Y EL ROL
    QModelIndex index = mModel->index(row, 0);
    bool favorite = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

    //CAMBIA A EL ROL "IsLikedRole" A LO CONTRARIO CUANDO SE PULSA
    mModel->setData(index, !favorite, ApplicationModel::IsLikedRole);

    //DEBUG
    QString name = mModel->data(index, ApplicationModel::NameRole).toString();
    qDebug() << "Boton Favorite clicado por: " << name;
}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
