#ifndef INSTALLEDAPPLICATIONSWIDGET_H
#define INSTALLEDAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include "installeditemwidget.h"
#include "applicationdelegate.h"
#include "filterproxymodel.h"
#include <QScrollBar>

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

signals:
    void infoClicked(const QModelIndex& index);
    void changeToMenuStyle(const bool& isMenuStyle);

public slots:
    void onSearchText(const QString& text);

private slots:
    void onLikedClicked(const QModelIndex& index);
    void onDeleteClicked(const QModelIndex& index);

    void onInstalledClicked();
    void onFavoriteClicked();

private:
    Ui::InstalledApplicationsWidget *ui;
    ApplicationModel *mModel;
    FilterProxyModel *mProxyModel;
    bool mShowFavorite = false;
};

#endif // INSTALLEDAPPLICATIONSWIDGET_H
