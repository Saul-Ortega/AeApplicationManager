#ifndef AVAILABLEAPPLICATIONSWIDGET_H
#define AVAILABLEAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include <QVector>

namespace Ui {
class AvailableApplicationsWidget;
}

class AvailableApplicationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableApplicationsWidget(QWidget *parent = nullptr);
    ~AvailableApplicationsWidget();

    void LoadWidget();

private slots:
    void onDownloadClicked(int row);
    void onFavoriteClicked(int row);
    void onInfoClicked(int row);

private:
    Ui::AvailableApplicationsWidget *ui;
    ApplicationModel *mModel;
    QVector<int> mVisibleRows;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
