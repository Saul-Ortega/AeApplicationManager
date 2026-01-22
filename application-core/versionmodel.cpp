#include "versionmodel.h"

#include "ApplicationDao.h".h"

//=== CONSTRUCTOR ===
VersionModel::VersionModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

//DEVUELVE EL NUMERO DE VERSIONES QUE TIENE LA APP
int VersionModel::rowCount(const QModelIndex &parent) const
{
    //DEVUELVE EL NUMERO DE VERSIONES
    return m_versions.size();
}

// DEVUELVE LA VERSION DE UNA FILA Y ROL
QVariant VersionModel::data(const QModelIndex &index, int role) const
{
    //SI EL INDICE NO ES VALIDO NO DEVUELVE NADA
    if (!index.isValid())
        return QVariant();

    //OBTIENE LA VERSION DE ESA FILA
    const Version& version = m_versions.at(index.row());

    //SEGUN EL ROL DEVUELVE UNA COSA U OTRA
    switch (role) {
    case IdRole:
        return version.id();
    case NameRole:
        return version.name();
    case SizeRole:
        return version.size();
    case LastModificationRole:
        return version.lastModification();
    case ExpirationDateRole:
        return version.expirationDate();
    case IsInstalledRole:
        return version.isInstalled();
    }

    return QVariant(); //NO DEVUELVE NADA SI EL ROL NO EXISTE
}

QHash<int, QByteArray> VersionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[LastModificationRole] = "lastModification";
    roles[ExpirationDateRole] = "expirationDate";
    roles[IsInstalledRole] = "isInstalled";
    return roles;
}

//REFRESCA CON LOS NUEVOS DATOS PARA LA VISTA
void VersionModel::setVersions(const QList<Version> &versions)
{
    beginResetModel();
    m_versions = versions;
    endResetModel();
}

//INSTALAMOS UNA NUEVA VERSION
void VersionModel::installVersion(int row) {

    // SI LA FILA NO EXISTE NO HACEMOS NADA
    if (row < 0 || row >= m_versions.size())
        return;

    //DESINSTALA TODAS LAS VERSIONES
    for (int i = 0; i < m_versions.size(); i++) {
        m_versions[i].setIsInstalled(false);
    }

    //INSTALAR LA VERSION SELECCIONADA
    m_versions[row].setIsInstalled(true);

    //REFRESCAMOS EL MODELO PARA LA VISTA
    beginResetModel();
    endResetModel();

    // ApplicationDao::updateApplication();
}

void VersionModel::uninstallVersion(int row){

    //SI LA FILA NO EXISTE NO HACEMOS NADA
    if (row <0 || row >=m_versions.size()) {
        return;
    }

    //DESINTSALAMOS LA VERSION SELECCIONADA
    m_versions[row].setIsInstalled(false);

    //REFRESCAMOS EL MODELO PARA LA VISTA
    beginResetModel();
    endResetModel();

    // ApplicationDao::updateApplication();
}
