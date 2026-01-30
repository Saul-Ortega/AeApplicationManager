#ifndef FILTERPROXYMODEL_H
#define FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FilterProxyModel(QObject* parent = nullptr);

    void setFilterText(const QString& text);
    void setShowOnlyFavorites(bool show);
    void setShowInstalled(bool show);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool mOnlyFavorites = false;
    bool mShowInstalled = false;
    QString mFilterText;
};

#endif // FILTERPROXYMODEL_H
