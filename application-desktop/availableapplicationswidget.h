#ifndef AVAILABLEAPPLICATIONSWIDGET_H
#define AVAILABLEAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include <QVector>
#include <QSortFilterProxyModel>
#include "filterproxymodel.h"

namespace Ui {
class AvailableApplicationsWidget;
}

class AvailableApplicationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableApplicationsWidget(QWidget *parent = nullptr);
    ~AvailableApplicationsWidget();

    // void LoadWidget();
    void setApplicationModel(FilterProxyModel* model);

signals:
    void infoClicked(const QModelIndex& index);

public slots:
    void onSearchText(const QString& text);


private slots:
    void onLikedClicked(const QModelIndex& index);

    void onAvailableClicked();
    void onFavoriteClicked();

    void onDownloadClicked(const QModelIndex& index);
    void onDownloadFinished(int row);

private:
    Ui::AvailableApplicationsWidget *ui;
    FilterProxyModel *mModel;
    bool mMostrarDeseados = false;
    // QMetaObject::Connection modelDataChangedConnect;

    // FilterProxyModel* mProxyModel;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
