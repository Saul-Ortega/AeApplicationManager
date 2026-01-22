#ifndef VERSIONMODEL_H
#define VERSIONMODEL_H

#include "application-core_global.h"
#include <QAbstractListModel>
#include "version.h"

//QABSTRACTLISTMODEL PARA EXPONER DATOS A LA VISTA
class APPLICATION_CORE_EXPORT VersionModel : public QAbstractListModel
{
    Q_OBJECT

public:

    //=== CONSTRUCTOR ===
    explicit VersionModel(QObject *parent = nullptr);

    //ROLES PARA ACCEDER A LOS DATOS
    enum VersionRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        SizeRole,
        LastModificationRole,
        ExpirationDateRole,
        IsInstalledRole
    };

    // === METODOS ===
    //DEVUELVE EL NUMERO DE VERSIONES DE UNA APP
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //DEVUELVE LA VERSION Y EL ROL QUE SOLICITE
    QVariant data(const QModelIndex &index, int role) const override;

    // ROLES
    QHash<int, QByteArray> roleNames() const override;

    // METODO PARA CARGAR DATOS
    void setVersions(const QList<Version> &versions);


public slots:
    //INSTALAR UNA VERSION
    void installVersion(int row);

    //DESINSTALAR UNA VERSION
    void uninstallVersion(int row);


    //LISTA DE VERSIONES
private:
    QList<Version> m_versions;
};

#endif // VERSIONMODEL_H
