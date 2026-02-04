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

    void setApplicationModel(ApplicationModel* model);

signals:
    void infoClicked(const QModelIndex& index);
    void changeToMenuStyle(const bool& isMenuStyle);

public slots:
    void onSearchText(const QString& text);


private slots:
    void onLikedClicked(const QModelIndex& index);

    void onAvailableClicked();
    void onFavoriteClicked();

    void onDownloadClicked(const QModelIndex& index);
    // void onDownloadFinished(int row);

private:
    Ui::AvailableApplicationsWidget *ui;
    ApplicationModel *mModel;
    FilterProxyModel *mProxyModel;
    bool mMostrarDeseados = false;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
