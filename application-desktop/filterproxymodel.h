#ifndef FILTERPROXYMODEL_H
#define FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "applicationmodel.h"
#include <QString>

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FilterProxyModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex& index, int role) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void setSourceModel(QAbstractItemModel* sourceModel) override;
    ApplicationModel* applicationModel() const;

    void setFilterText(const QString& text);
    void setShowOnlyFavorites(bool show);
    void setShowInstalled(bool show);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool mOnlyFavorites;
    bool mShowInstalled;
    QString mFilterText;
};

#endif // FILTERPROXYMODEL_H
