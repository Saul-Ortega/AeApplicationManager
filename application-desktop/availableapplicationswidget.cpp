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
    ui->listViewAvailable->setSpacing(10);
    ui->listViewAvailable->setResizeMode(QListView::Adjust);
    ui->listViewAvailable->setFlow(QListView::LeftToRight);
    ui->listViewAvailable->setWrapping(true);
    ApplicationDelegate* delegate = new ApplicationDelegate();
    ui->listViewAvailable->setItemDelegate(delegate);

    //ACTIVAR EL FUNCIONAMIENTO DE LOS BOTONES EN LA LISTA
    // ui->listViewAvailable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ui->listViewAvailable->setSelectionMode(QAbstractItemView::NoSelection);
    // ui->listViewAvailable->setFocusPolicy(Qt::NoFocus);


    connect(delegate, &ApplicationDelegate::isLikedButtonClicked, this, &AvailableApplicationsWidget::onLikedClicked);
    connect(delegate, &ApplicationDelegate::infoButtonClicked, this, &AvailableApplicationsWidget::infoClicked);
    connect(delegate, &ApplicationDelegate::isDownloadedButtonClicked, this, &AvailableApplicationsWidget::onDownloadClicked);

    //BOTON YA MARCADO
    ui->btn_Disponibles->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");

    // FONDO EN BLANCO
    ui->frame->setStyleSheet("background-color: #FFFFFF;");
    ui->listViewAvailable->setStyleSheet("background-color: #FFFFFF; border: none;");

    connect(ui->btn_Disponibles, &QPushButton::clicked, this, &AvailableApplicationsWidget::onAvailableClicked);
    connect(ui->btn_Deseados, &QPushButton::clicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
}


void AvailableApplicationsWidget::setApplicationModel(FilterProxyModel* model)
{
    mModel = model;
    mModel->setShowInstalled(false);
    ui->listViewAvailable->setModel(mModel);

    // if (modelDataChangedConnect) disconnect(modelDataChangedConnect);

    // modelDataChangedConnect = connect(mModel, &ApplicationModel::dataChanged, this, [this] () {
    //     qDebug() << "DataChanged funcionó";
    //     LoadWidget();
    // });

    // //CREAMOS EL PROXYMODEL
    // mProxyModel = new FilterProxyModel(this);

    // //CONECTAMOS EL PROXY AL MODELO
    // mProxyModel->setSourceModel(mModel);

    // //POR DEFAULT MUESTRA TODAS LAS APPS NO INSTALADAS
    // mProxyModel->setShowInstalled(false);

    //CONECTAMOS EL PROXY A LA VISTA

// AQUI IRÁ EL DELEGATE CREO Y QUITAR EL LOADWIDGET DEL DATACHANGED
}


// === BUTTONS ===


// CAMBIAMOS EL ROL DE FAVORITOS Y ACTUALIZAMOS EL CORAZON
void AvailableApplicationsWidget::onLikedClicked(const QModelIndex& index)
{
    bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();
    mModel->setData(index, !isLiked, ApplicationModel::IsLikedRole);
}

//ACTIVAMOS EL FILTRO DE TODOS LOS DISPONIBLES
void AvailableApplicationsWidget::onAvailableClicked()
{
    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Disponibles->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->btn_Deseados->setStyleSheet("");

    //SI PULSA EL BOTON NO SOLO MOSTRARA LOS FAVORITOS
    if(mModel){
        mModel->setShowOnlyFavorites(false);
    }
    //LLAMAMOS AL METODO PARA GENERAR LOS WIDGETS
    // LoadWidget();
}

// ACTIVAMOS EL FILTRO DE SOLO LOS DESEADOS
void AvailableApplicationsWidget::onFavoriteClicked(){

    //ALTERNAR LOS COLORES AL PULSAR
    ui->btn_Deseados->setStyleSheet("background-color: #E0E0E0; font-weight: bold;");
    ui->btn_Disponibles->setStyleSheet("");

    //SI PULSA EL BOTON SOLO MOSTRARA LOS FAVORITOS
    if(mModel){
        mModel->setShowOnlyFavorites(true);
    }

    //LLAMAMOS AL METODO PARA GENERAR LOS WIDGETS
    // AvailableApplicationsWidget::LoadWidget();
}


// MODIFICAMOS EL ROL DE LA DESCARGA DE LA APP
void AvailableApplicationsWidget::onDownloadClicked(const QModelIndex& index)
{
    // QString name = mModel->data(mModel->index(row, 0), ApplicationModel::NameRole).toString();
    // qDebug() << "Boton Download clicado por:" << name;

    // //RECIBE EL INDEX DE LA FILA Y EL ROL // QModelIndex appIndex = mModel->index(row,0);


    //MODIFICAMOS EL ROL APPLICATION "IsDownloadRole" A TRUE CUANDO SE PULSA
    mModel->setData(index, true, ApplicationModel::IsDownloadedRole);

    //GUARDAMOS LAS VERSIONES EN QLIST
    bool t = mModel->data(index, ApplicationModel::IsLikedRole).toBool();
    QList<Version> versions = mModel->data(index, ApplicationModel::VersionsRole).value<QList<Version>>();

    //MODIFICAMOS EL ROL DE VERSION "IsInstalledRole" A TRUE LA ULTIMA VERSION
    if(!versions.empty()){
        versions.last().setIsInstalled(true);
    }

    //GUARDAMOS LAS VERSIONES MODIFICADAS AL MODELO
    mModel->setData(index, QVariant::fromValue(versions), ApplicationModel::VersionsRole);

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
    if(mModel){
        mModel->setFilterText(text);
    }
}


// === LOAD WIDGETS ===

// void AvailableApplicationsWidget::LoadWidget(){
//     // LIMPIAR WIDGETS ANTERIORES SI EXISTEN
//     for (int i = 0; i < mModel->rowCount(); i++) {

//         //OBTENEMOS EL INDEX DEL MODEL
//         QModelIndex index = mModel->index(i, 0);

//         //COMPROBAMOS EN EL LIST SI EN ESA FILA HAY ALGUN WIDGET ASOCIADO DE ANTES
//         QWidget* oldWidget = ui->listViewAvailable->indexWidget(index);

//         //SI EL PUNTERO APUNTA A ALGO LO ELIMINA
//         if (oldWidget) {
//             oldWidget->disconnect(); //PRIMERO LO DESCONECTA
//             ui->listViewAvailable->setIndexWidget(index, nullptr);  //LUEGO LO QUITAMOS DE LA LISTA
//             oldWidget->deleteLater();  //POR ULTIMO LO ELIMINAMOS
//         }
//     }

//     // INSERTAMOS EL WIDGET POR CADA APLICACION
//     for (int i = 0; i < mModel->rowCount(); i++) {
//         QModelIndex index = mModel->index(i, 0);

//         //FILTRAMOS LAS APPS NO INSTALADAS
//         bool isDownload = mModel->data(index, ApplicationModel::IsDownloadedRole).toBool();
//         bool isLiked = mModel->data(index, ApplicationModel::IsLikedRole).toBool();

//         bool mostrar;

//         mMostrarDeseados ? mostrar = !isDownload&&isLiked : mostrar = !isDownload;

//         if (mostrar) {

//             // CREAR Y MOSTRAR WIDGET PARA APPS NO DESCARGADAS
//             AvailableItemWidget *widget = new AvailableItemWidget();

//             // PASAMOS EL MODELO AL WIDGET
//             widget->setModel(mModel);

//             // OBTENEMOS EL NOMBRE, LA IMAGEN Y EL ESTADO DE FAVORITO
//             widget->setData(index);

//             // ASIGNAMOS LA FILA AL WIDGET
//             widget->setRow(i);

//             // INSERTAMOS EL WIDGET EN LA VISTA
//             ui->listViewAvailable->setIndexWidget(index, widget);

//             // MOSTRAR LA FILA
//             ui->listViewAvailable->setRowHidden(i, false);

//             // CONNECTS
//             // connect(widget, &AvailableItemWidget::favoriteClicked, this, &AvailableApplicationsWidget::onFavoriteClicked);
//             // connect(widget, &AvailableItemWidget::downloadClicked, this, &AvailableApplicationsWidget::onDownloadClicked);
//             // connect(widget, &AvailableItemWidget::infoClicked, this, &AvailableApplicationsWidget::infoClicked);

//             // connect(widget, &AvailableItemWidget::downloadFinished, this, &AvailableApplicationsWidget::onDownloadFinished);
//         } else {
//             // OCULTAR LA FILA DE APPS DESCARGADAS
//             ui->listViewAvailable->setRowHidden(i, true);
//         }
//     }
// }

AvailableApplicationsWidget::~AvailableApplicationsWidget()
{
    delete ui;
}
