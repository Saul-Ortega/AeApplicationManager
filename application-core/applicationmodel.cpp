#include "applicationmodel.h"

//CONSTRUCTOR
ApplicationModel::ApplicationModel(QObject* parent)
    : QAbstractListModel(parent)
    //TODO: AÑADIR EL DAO.APPLICATIONS CUANDO ESTÉ HECHO
    , mApplications()
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

    //TODO: LLAMAR AL DAO PARA ACTUALIZAR LA APLICACIÓN
    emit dataChanged(index, index);
    return true;
}

QModelIndex ApplicationModel::addApplication(const Application& application)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    std::unique_ptr<Application> newApplication(new Application(application));
    //TODO: LLAMAR AL DAO PARA INSERTAR LA APLICACIÓN
    mApplications->push_back(std::move(newApplication));
    endInsertRows();
    return index(rowIndex, 0);
}

int ApplicationModel::rowCount(const QModelIndex& parent) const
{
    return mApplications->size();
}

bool ApplicationModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if ( row < 0 || row >= rowCount() || count < 0 || (row + count) > rowCount() ) {
        return false;
    }

    beginRemoveRows(parent, row, row + count -1);
    int countLeft = count;
    while ( countLeft-- ) {
        const Application& application = *mApplications->at(row + countLeft);
        //TODO: LLAMAR AL DAO PARA ELIMINAR EL ALBUM POR ID
    }
    mApplications->erase(mApplications->begin() + row, mApplications->begin() + row + count);
    endRemoveRows();
    return true;
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
