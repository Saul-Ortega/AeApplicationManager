#ifndef APPLICATIONMANAGERWIDGET_H
#define APPLICATIONMANAGERWIDGET_H

#include <QWidget>
#include "QItemSelectionModel"
#include "applicationmodel.h"
#include "versionmodel.h"
#include "filterproxymodel.h"

namespace Ui {
class ApplicationManagerWidget;
}

class ApplicationManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationManagerWidget(QWidget *parent = nullptr);
    ~ApplicationManagerWidget();

    void setApplicationModel(FilterProxyModel* model);
    void setApplicationSelectionModel(QItemSelectionModel* applicationSelectionModel);
    void setVersionModel(VersionModel* versionModel);
    void setVersionSelectionModel(QItemSelectionModel* versionSelectionModel);

private slots:
    void onInfoClicked(const QModelIndex& index);

private:
    Ui::ApplicationManagerWidget *ui;
    FilterProxyModel* mModel;
    VersionModel* mVersionModel;
};

#endif // APPLICATIONMANAGERWIDGET_H
