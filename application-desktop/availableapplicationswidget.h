#ifndef AVAILABLEAPPLICATIONSWIDGET_H
#define AVAILABLEAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include "ApplicationDelegate.h"

namespace Ui {
class AvailableApplicationsWidget;
}

class AvailableApplicationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableApplicationsWidget(QWidget *parent = nullptr);
    ~AvailableApplicationsWidget();

private slots:
    void onInfoClicked(int row);
    void onFavoriteClicked(int row);
    void onDeleteClicked(int row);

private:
    Ui::AvailableApplicationsWidget *ui;
    ApplicationModel *mModel;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
