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
        widget->setData(
            mModel->data(index, ApplicationModel::NameRole).toString(),
            mModel->data(index, ApplicationModel::ImageUrlRole).toString()
            );

        // ASIGNAMOS LA FILA AL WIDGET
        widget->setRow(i);

        // INSERTAMOS EL WIDGET EN LA VISTA
        ui->listViewAvailable->setIndexWidget(index, widget);

        // CONNECTS
        connect(widget, &AvailableItemWidget::infoClicked, this, &AvailableApplicationsWidget::onInfoClicked);
        connect(widget, &AvailableItemWidget::favoriteClicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
        connect(widget, &AvailableItemWidget::deleteClicked, this, &AvailableApplicationsWidget::onDeleteClicked);
    }

    // FONDO EN BLANCO
    ui->frame->setStyleSheet("background-color: #FFFFFF;");
    ui->listViewAvailable->setStyleSheet("background-color: #FFFFFF; border: none;");
}

void AvailableApplicationsWidget::onInfoClicked(int row)
{
    QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    qDebug() << "Boton Info clicado por:" << name << "Row:" << row;
}

void AvailableApplicationsWidget::onFavoriteClicked(int row)
{
    QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    qDebug() << "Boton Favorite clicado por:" << name << "Row:" << row;
}

void AvailableApplicationsWidget::onDeleteClicked(int row)
{
    QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    qDebug() << "Boton Delete clicado por:" << name << "Row:" << row;
}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
