#include "applicationmodel.h"

//CONSTRUCTOR
ApplicationModel::ApplicationModel(QObject* parent)
    : QAbstractListModel(parent)
    //TODO: MODIFICAR PARA QUE TENGA LA RUTA AL JSON
    , mApplicationDao()
    , mApplications(mApplicationDao.applications())
{
}

//MÉTODOS
QVariant ApplicationModel::data(const QModelIndex& index, int role) const
{
    if ( !index.isValid() ) {
        return QVariant();
    }

    const Application& application = *mApplications->at(index.row());

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
    case ApplicationRoles::IsDownloadedRole :
        return application.isDownloaded();
    case ApplicationRoles::VersionsRole :
        return QVariant::fromValue(application.versions());
    default :
        return QVariant();
    }
}

bool ApplicationModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    //COMPRUEBA QUE EL ÍNDICE SEA VÁLIDO Y EL ROL SEA ALGUNO DE LOS DEFINIDOS ANTERIORMENTE
    if ( !index.isValid()
        || role != ApplicationRoles::NameRole
        || role != ApplicationRoles::DescriptionRole
        || role != ApplicationRoles::ImageUrlRole
        || role != ApplicationRoles::ExecutableFileRole
        || role != ApplicationRoles::ExpirationDateRole
        || role != ApplicationRoles::IsLikedRole
        || role != ApplicationRoles::IsDownloadedRole
        || role != ApplicationRoles::VersionsRole) {
        return false;
    }

    Application& application = *mApplications->at(index.row());

    switch ( role ) {
        case ApplicationRoles::NameRole :
            application.setName(value.toString());
        case ApplicationRoles::DescriptionRole :
            application.setDescription(value.toString());
        case ApplicationRoles::ImageUrlRole :
            application.setImageUrl(value.toString());
        case ApplicationRoles::ExecutableFileRole :
            application.setExecutableFile(value.toString());
        case ApplicationRoles::ExpirationDateRole :
            application.setExpirationDate(value.toDate());
        case ApplicationRoles::IsLikedRole :
            application.setIsLiked(value.toBool());
        case ApplicationRoles::IsDownloadedRole :
            application.setIsDownloaded(value.toBool());
        case ApplicationRoles::VersionsRole :
            application.setVersions(value.value<QList<Version>>());
    }

    mApplicationDao.updateApplication(application);
    emit dataChanged(index, index);
    return true;
}

int ApplicationModel::rowCount(const QModelIndex& parent) const
{
    return mApplications->size();
}

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
    roles[ApplicationRoles::IsDownloadedRole] = "isDownloaded";
    roles[ApplicationRoles::VersionsRole] = "versions";
    return roles;
}
