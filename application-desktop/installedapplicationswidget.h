#ifndef INSTALLEDAPPLICATIONSWIDGET_H
#define INSTALLEDAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include "installeditemwidget.h"

namespace Ui {
class InstalledApplicationsWidget;
}

class InstalledApplicationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstalledApplicationsWidget(QWidget *parent = nullptr);
    ~InstalledApplicationsWidget();

    void setApplicationModel(ApplicationModel* model);
    void loadWidgets();

signals:
    void infoClicked(const QModelIndex& index);

private slots:
    void onLikedClicked(int row);
    void onDeleteClicked(int row);

    void onInstalledClicked();
    void onFavoriteClicked();

private:
    Ui::InstalledApplicationsWidget *ui;
    ApplicationModel *mModel;
    bool mShowFavorite = false;
    QMetaObject::Connection mDataChangedConnection;
};

#endif // INSTALLEDAPPLICATIONSWIDGET_H
