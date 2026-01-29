#include "installedapplicationswidget.h"
#include "ui_installedapplicationswidget.h"

InstalledApplicationsWidget::InstalledApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InstalledApplicationsWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

    // // CREAR MODELO
    // mModel = new ApplicationModel(this);

    // // ASIGNAR MODELO A LA VISTA
    // ui->listViewInstalled->setModel(mModel);

    // CONFIGURACION DEL LISTVIEW
    ui->listViewInstalled->setViewMode(QListView::ListMode);
    ui->listViewInstalled->setWrapping(true);
    ui->listViewInstalled->setResizeMode(QListView::Adjust);
    ui->listViewInstalled->setUniformItemSizes(true);
    ui->listViewInstalled->setSpacing(10);
    ui->listViewInstalled->setFlow(QListView::LeftToRight);
    ui->listViewInstalled->setGridSize(QSize(200, 170));

    //ACTIVAR EL FUNCIONAMIENTO DE LOS BOTONES EN LA LISTA
    ui->listViewInstalled->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listViewInstalled->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listViewInstalled->setFocusPolicy(Qt::NoFocus);

    connect(ui->installedBtn, &QPushButton::clicked, this, &InstalledApplicationsWidget::onInstalledClicked);
    connect(ui->favoriteBtn, &QPushButton::clicked, this, &InstalledApplicationsWidget::onFavoriteClicked);
}

InstalledApplicationsWidget::~InstalledApplicationsWidget()
{
    delete ui;
}

void InstalledApplicationsWidget::setApplicationModel(ApplicationModel* model)
{
    mModel = model;
    ui->listViewInstalled->setModel(mModel);
    if ( mDataChangedConnection ) disconnect(mDataChangedConnection);
    mDataChangedConnection = connect(mModel, &ApplicationModel::dataChanged, this, [this] () {
        loadWidgets();
    });

    loadWidgets();
}

void InstalledApplicationsWidget::loadWidgets()
{
    // LIMPIAR WIDGETS ANTERIORES SI EXISTEN
    for (int i = 0; i < mModel->rowCount(); i++) {

        //OBTENEMOS EL INDEX DEL MODEL
        QModelIndex index = mModel->index(i, 0);

        //COMPROBAMOS EN EL LIST SI EN ESA FILA HAY ALGUN WIDGET ASOCIADO DE ANTES
        QWidget* oldWidget = ui->listViewInstalled->indexWidget(index);

        //SI EL PUNTERO APUNTA A ALGO LO ELIMINA
        if (oldWidget) {
            oldWidget->disconnect(); //PRIMERO LO DESCONECTA
            ui->listViewInstalled->setIndexWidget(index, nullptr);  //LUEGO LO QUITAMOS DE LA LISTA
            oldWidget->deleteLater();  //POR ULTIMO LO ELIMINAMOS
        }
    }

    // INSERTAMOS EL WIDGET POR CADA APLICACION
    for (int i = 0; i < mModel->rowCount(); i++) {
        QModelIndex index = mModel->index(i, 0);

        //FILTRAMOS LAS APPS INSTALADAS
        bool isDownload = mModel->data(index, ApplicationModel::IsDownloadedRole).toBool();
        bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

        bool show;

        mShowFavorite ? show = isDownload&&isLiked : show = isDownload;

        if (show) {

            // CREAR Y MOSTRAR WIDGET PARA APPS NO DESCARGADAS
            InstalledItemWidget *widget = new InstalledItemWidget();

            // PASAMOS EL MODELO AL WIDGET
            widget->setModel(mModel);

            // OBTENEMOS EL NOMBRE, LA IMAGEN Y EL ESTADO DE FAVORITO
            widget->setData(index);

            // ASIGNAMOS LA FILA AL WIDGET
            widget->setRow(i);

            // INSERTAMOS EL WIDGET EN LA VISTA
            ui->listViewInstalled->setIndexWidget(index, widget);

            // MOSTRAR LA FILA
            ui->listViewInstalled->setRowHidden(i, false);

            // CONNECTS
            connect(widget, &InstalledItemWidget::likedClicked, this, &InstalledApplicationsWidget::onLikedClicked);
            connect(widget, &InstalledItemWidget::infoClicked, this, &InstalledApplicationsWidget::infoClicked);
            connect(widget, &InstalledItemWidget::deleteClicked, this, &InstalledApplicationsWidget::onDeleteClicked);
        } else {
            // OCULTAR LA FILA DE APPS DESCARGADAS
            ui->listViewInstalled->setRowHidden(i, true);
        }
    }
}

void InstalledApplicationsWidget::onDeleteClicked(int row)
{
    QModelIndex index = mModel->index(row, 0);
    //ELIMINA LA APLICACIÓN
    mModel->setData(index, false, ApplicationModel::IsDownloadedRole);

    //ELIMINA TODAS LAS VERSIONES DE DICHA APLICACIÓN
    QList<Version> versions = mModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    for ( int i = 0; i < versions.size(); i++ ) {
        versions[i].setIsInstalled(false);
    }

    mModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);
}

void InstalledApplicationsWidget::onLikedClicked(int row)
{
    QModelIndex index = mModel->index(row, 0);
    QString name = mModel->data(index, ApplicationModel::NameRole).toString();

    // CAMBIAR EL ESTADO DE FAVORITO DEL MODELO
    bool currentState = mModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mModel->setData(index, !currentState, ApplicationModel::IsLikedRole);

    //OBTENEMOS EL ITEMWIDGET QUE EL USUARIO PULSO PARA TENER EN FAVORITOS
    InstalledItemWidget* widget = (InstalledItemWidget*)ui->listViewInstalled->indexWidget(index);

    //SI EL WIDGET EXISTE ACTUALIZA EL CORAZON
    if (widget) {
        widget->setData(index);
    }
}

void InstalledApplicationsWidget::onInstalledClicked()
{
    mShowFavorite = false;
    loadWidgets();
}

void InstalledApplicationsWidget::onFavoriteClicked()
{
    mShowFavorite = true;
    loadWidgets();
}
