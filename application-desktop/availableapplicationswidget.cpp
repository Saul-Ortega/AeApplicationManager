#include "availableapplicationswidget.h"
#include "ui_availableapplicationswidget.h"
#include "applicationmodel.h"
#include "AvailableItemWidget.h"
#include "applicationinfodialog.h"
#include "filterproxymodel.h"
#include <QTimer>

AvailableApplicationsWidget::AvailableApplicationsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AvailableApplicationsWidget)
    , mModel(nullptr)
{
    ui->setupUi(this);

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

    connect(ui->btn_Disponibles, &QPushButton::clicked, this, &AvailableApplicationsWidget::onDisponiblesClicked);
    connect(ui->btn_Deseados, &QPushButton::clicked, this, &AvailableApplicationsWidget::onDeseadosClicked);

    //BOTON YA MARCADO
    ui->btn_Disponibles->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");

    // FONDO EN BLANCO
    ui->frame->setStyleSheet("background-color: #FFFFFF;");
    ui->listViewAvailable->setStyleSheet("background-color: #FFFFFF; border: none;");
}


void AvailableApplicationsWidget::setApplicationModel(ApplicationModel* model)
{
    mModel = model;

    if (modelDataChangedConnect) disconnect(modelDataChangedConnect);

    modelDataChangedConnect = connect(mModel, &ApplicationModel::dataChanged, this, [this] () {
        qDebug() << "DataChanged funcionó";
        LoadWidget();
    });

    //CREAMOS EL PROXYMODEL
    mProxyModel = new FilterProxyModel(this);

    //CONECTAMOS EL PROXY AL MODELO
    mProxyModel->setSourceModel(mModel);

    //POR DEFAULT MUESTRA TODAS LAS APPS NO INSTALADAS
    mProxyModel->setShowInstalled(false);

    //CONECTAMOS EL PROXY A LA VISTA
    ui->listViewAvailable->setModel(mProxyModel);

// AQUI IRÁ EL DELEGATE CREO Y QUITAR EL LOADWIDGET DEL DATACHANGED
}


// === BUTTONS ===


// CAMBIAMOS EL ROL DE FAVORITOS Y ACTUALIZAMOS EL CORAZON
void AvailableApplicationsWidget::onFavoriteClicked(int row)
{
    QModelIndex index = mModel->index(row, 0);
    QString name = mModel->data(index, ApplicationModel::NameRole).toString();
    qDebug() << "Boton Favorite clicado por: " << name;

    // CAMBIAR EL ESTADO DE FAVORITO DEL MODELO
    bool currentState = mModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mModel->setData(index, !currentState, ApplicationModel::IsLikedRole);

    //OBTENEMOS EL ITEMWIDGET QUE EL USUARIO PULSO PARA TENER EN FAVORITOS
    AvailableItemWidget* widget = (AvailableItemWidget*)ui->listViewAvailable->indexWidget(index);

    //SI EL WIDGET EXISTE ACTUALIZA EL CORAZON
    if (widget) {
        widget->setData(index);
    }

    // SI ESTA EN EL FILTRO DE DESEADOS ACTUALIZA LA LISTA PARA QUE DESAPAREZCA EL WIDGET
    if (mMostrarDeseados) {
        LoadWidget();
    }
}

//ACTIVAMOS EL FILTRO DE TODOS LOS DISPONIBLES
void AvailableApplicationsWidget::onDisponiblesClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Disponibles->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->btn_Deseados->setStyleSheet("");

    //SI PULSA EL BOTON NO SOLO MOSTRARA LOS FAVORITOS
    if(mProxyModel){
        mProxyModel->setShowOnlyFavorites(false);
    }
    //LLAMAMOS AL METODO PARA GENERAR LOS WIDGETS
    LoadWidget();
}

// ACTIVAMOS EL FILTRO DE SOLO LOS DESEADOS
void AvailableApplicationsWidget::onDeseadosClicked(){

    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Deseados->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->btn_Disponibles->setStyleSheet("");

    //SI PULSA EL BOTON SOLO MOSTRARA LOS FAVORITOS
    if(mProxyModel){
        mProxyModel->setShowOnlyFavorites(true);
    }

    //LLAMAMOS AL METODO PARA GENERAR LOS WIDGETS
    AvailableApplicationsWidget::LoadWidget();
}


// MODIFICAMOS EL ROL DE LA DESCARGA DE LA APP
void AvailableApplicationsWidget::onDownloadClicked(int row)
{
    QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    qDebug() << "Boton Download clicado por:" << name;

    //RECIBE EL INDEX DE LA FILA Y EL ROL
    QModelIndex appIndex = mModel->index(row,0);


    //MODIFICAMOS EL ROL APPLICATION "IsDownloadRole" A TRUE CUANDO SE PULSA
    mModel->setData(appIndex, true, ApplicationModel::IsDownloadedRole);

    //GUARDAMOS LAS VERSIONES EN QLIST
    QList<Version> versions = mModel->data(appIndex, ApplicationModel::VersionsRole).value<QList<Version>>();

    //MODIFICAMOS EL ROL DE VERSION "IsInstalledRole" A TRUE LA ULTIMA VERSION
    if(!versions.empty()){
        versions.last().setIsInstalled(true);
    }

    //GUARDAMOS LAS VERSIONES MODIFICADAS AL MODELO
    mModel->setData(appIndex, QVariant::fromValue(versions), ApplicationModel::VersionsRole);

}

// CUANDO LA DESCARGA FINALIZA ELIMINAMOS EL WIDGET
void AvailableApplicationsWidget::onDownloadFinished(int row)
{
    QModelIndex index = mModel->index(row, 0);
    QString name = mModel->data(index, ApplicationModel::NameRole).toString();
    qDebug() << "Descarga finalizada:" << name;

    // RECIBIMOS EL WIDGET QUE TERMINO LA DESCARGA
    QWidget* widget = ui->listViewAvailable->indexWidget(index);

    if (widget) {
        // SI EXISTE EL WIDGET LO DESCONECTAMOS
        widget->disconnect();

        // QUITAMOS EL WIDGET DE LA LISTA
        ui->listViewAvailable->setIndexWidget(index, nullptr);

        // OCULTAMOS LA FILA PARA QUE OTRO WIDGET OCUPE SU LUGAR
        ui->listViewAvailable->setRowHidden(row, true);

        // ELIMINAMOS EL WIDGET
        widget->deleteLater();
    }
}

void AvailableApplicationsWidget::onSearchText(const QString& text) {

    if(mProxyModel){
        mProxyModel->setFilterText(text);
    }
}


// === LOAD WIDGETS ===

void AvailableApplicationsWidget::LoadWidget(){
    // LIMPIAR WIDGETS ANTERIORES SI EXISTEN
    for (int i = 0; i < mModel->rowCount(); i++) {

        //OBTENEMOS EL INDEX DEL MODEL
        QModelIndex index = mModel->index(i, 0);

        //COMPROBAMOS EN EL LIST SI EN ESA FILA HAY ALGUN WIDGET ASOCIADO DE ANTES
        QWidget* oldWidget = ui->listViewAvailable->indexWidget(index);

        //SI EL PUNTERO APUNTA A ALGO LO ELIMINA
        if (oldWidget) {
            oldWidget->disconnect(); //PRIMERO LO DESCONECTA
            ui->listViewAvailable->setIndexWidget(index, nullptr);  //LUEGO LO QUITAMOS DE LA LISTA
            oldWidget->deleteLater();  //POR ULTIMO LO ELIMINAMOS
        }
    }

    // INSERTAMOS EL WIDGET POR CADA APLICACION
    for (int i = 0; i < mModel->rowCount(); i++) {
        QModelIndex index = mModel->index(i, 0);

        //FILTRAMOS LAS APPS NO INSTALADAS
        bool isDownload = mModel->data(index, ApplicationModel::IsDownloadedRole).toBool();
        bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

        bool mostrar;

        mMostrarDeseados ? mostrar = !isDownload&&isLiked : mostrar = !isDownload;

        if (mostrar) {

            // CREAR Y MOSTRAR WIDGET PARA APPS NO DESCARGADAS
            AvailableItemWidget *widget = new AvailableItemWidget();

            // PASAMOS EL MODELO AL WIDGET
            widget->setModel(mModel);

            // OBTENEMOS EL NOMBRE, LA IMAGEN Y EL ESTADO DE FAVORITO
            widget->setData(index);

            // ASIGNAMOS LA FILA AL WIDGET
            widget->setRow(i);

            // INSERTAMOS EL WIDGET EN LA VISTA
            ui->listViewAvailable->setIndexWidget(index, widget);

            // MOSTRAR LA FILA
            ui->listViewAvailable->setRowHidden(i, false);

            // CONNECTS
            connect(widget, &AvailableItemWidget::favoriteClicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
            connect(widget, &AvailableItemWidget::downloadClicked, this, &AvailableApplicationsWidget::onDownloadClicked);
            connect(widget, &AvailableItemWidget::infoClicked, this, &AvailableApplicationsWidget::infoClicked);

            connect(widget, &AvailableItemWidget::downloadFinished, this, &AvailableApplicationsWidget::onDownloadFinished);
        } else {
            // OCULTAR LA FILA DE APPS DESCARGADAS
            ui->listViewAvailable->setRowHidden(i, true);
        }
    }
}

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
