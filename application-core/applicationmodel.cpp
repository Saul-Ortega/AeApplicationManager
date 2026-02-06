#include "applicationmodel.h"
#include <QSize>

//CONSTRUCTOR
ApplicationModel::ApplicationModel(QObject* parent)
    : QAbstractListModel(parent)
    , mApplicationDao()
    , mApplications(mApplicationDao.applications())
{
}

//METODOS
QVariant ApplicationModel::data(const QModelIndex& index, int role) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    //OBTENEMOS LA PALICACION MEDIANTE LA FILA
    const Application& application = *mApplications->at(index.row());

    //DEVOLVEMOS EL VALOR SEGUN EL ROL SOLICITADO
    switch ( role ) {
    case ApplicationRoles::IdRole :
        return application.id();
    case ApplicationRoles::NameRole :
    case Qt::DisplayRole :
        return application.name();
    case ApplicationRoles::DescriptionRole :
        return application.description();
    case ApplicationRoles::ImageUrlRole :
        return application.imageUrl();
    case ApplicationRoles::ExecutableFileRole :
        return application.executableFile();
    case ApplicationRoles::ExpirationDateRole :
        return application.expirationDate();
    case ApplicationRoles::IsLikedRole :
        return application.isLiked();
    case ApplicationRoles::UpdateRole :
        return application.update();
    case ApplicationRoles::IsDownloadedRole :
        return application.isDownloaded();
    case ApplicationRoles::VersionsRole :
        return QVariant::fromValue(application.versions());
    case Qt::SizeHintRole:
        return QSize(180, 170);
    case ApplicationModel::ProgressRole:
        return application.progress();

    default :
        return QVariant();
    }
}

bool ApplicationModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    //OBTENEMOS LA APLICACION A MODIFICAR
    Application& application = *mApplications->at(index.row());

    //ACTUALIZAMOS EL CAMPO SEGUN EL ROL
    switch (role) {
    case NameRole:
        application.setName(value.toString());
        break;
    case DescriptionRole:
        application.setDescription(value.toString());
        break;
    case ImageUrlRole:
        application.setImageUrl(value.toString());
        break;
    case ExecutableFileRole:
        application.setExecutableFile(value.toString());
        break;
    case ExpirationDateRole:
        application.setExpirationDate(value.toDate());
        break;
    case IsLikedRole:
        application.setIsLiked(value.toBool());
        break;
    case UpdateRole:
        application.setUpdate(value.toBool());
        break;
    case IsDownloadedRole:
        application.setIsDownloaded(value.toBool());
        break;
    case VersionsRole:
        application.setVersions(value.value<QList<Version>>());
        break;
    case ProgressRole:{
        beginResetModel();
        application.setProgress(value.toInt());
        endResetModel();
        return true; //para no aplicar cambios en el json
    }

    default:
        return false;
    }

    //GUARDAMOS LOS CAMBIOS EN DAO
    mApplicationDao.updateApplication(application);

    //ACTUALIZAR LA VISTA DEL QLISTVIEW
    emit dataChanged(index, index, {role});
    return true;
}

    //DEVUELVE CUANTAS APLICACIONES HAY EN EL MODELO
int ApplicationModel::rowCount(const QModelIndex& parent) const
{
    return mApplications->size();
}

    //ASOCIAMOS CADA ROL CON UN NOMBRE
QHash<int, QByteArray> ApplicationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ApplicationRoles::IdRole] = "id";
    roles[ApplicationRoles::NameRole] = "name";
    roles[ApplicationRoles::DescriptionRole] = "description";
    roles[ApplicationRoles::ImageUrlRole] = "imageUrl";
    roles[ApplicationRoles::ExecutableFileRole] = "executableFile";
    roles[ApplicationRoles::ExpirationDateRole] = "expirationDate";
    roles[ApplicationRoles::IsLikedRole] = "isLiked";
    roles[ApplicationRoles::UpdateRole] = "update";
    roles[ApplicationRoles::IsDownloadedRole] = "isDownloaded";
    roles[ApplicationRoles::VersionsRole] = "versions";
    roles[ApplicationRoles::ProgressRole] = "progress";

    return roles;
}

QModelIndex ApplicationModel::indexForAppId(int appId) const
{
    //REVISAMOS TODAS LAS APLICACIONES
    for (int row=0; row<mApplications->size(); row++){

        //SI LA APLICACION DE ESA FILA TIENE EL MISMO ID QUE EL SELECCIONADO
        if (mApplications->at(row)->id()==appId) {
            return index(row);
        }
    }
    // DEVOLVEMOS EL ÍNDICE VACÍO SI NO ENCONTRÓ NINGUNO
    return QModelIndex();
}
