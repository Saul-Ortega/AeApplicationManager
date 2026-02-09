#ifndef APPLICATIONMANAGERWIDGET_H
#define APPLICATIONMANAGERWIDGET_H

#include <QWidget>
#include "QItemSelectionModel"
#include "applicationmodel.h"
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

    void setApplicationModel(ApplicationModel* model);

private slots:
    void onInfoClicked(const QModelIndex& index);

private:
    Ui::ApplicationManagerWidget *ui;
    ApplicationModel* mModel;
};

#endif // APPLICATIONMANAGERWIDGET_H
