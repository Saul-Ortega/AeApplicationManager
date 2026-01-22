#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>

#include "application-core_global.h"
#include "application.h"
#include "applicationdao.h"

class APPLICATION_CORE_EXPORT ApplicationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ApplicationRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        ImageUrlRole,
        ExecutableFileRole,
        ExpirationDateRole,
        IsLikedRole,
        IsDownloadedRole,
        VersionsRole
    };

    ApplicationModel(QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

private:
    ApplicationDao mApplicationDao;
    std::unique_ptr<std::vector<std::unique_ptr<Application>>> mApplications;
};

#endif // APPLICATIONMODEL_H
