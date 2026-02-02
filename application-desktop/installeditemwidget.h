#ifndef INSTALLEDITEMWIDGET_H
#define INSTALLEDITEMWIDGET_H

#include <QWidget>
#include "applicationmodel.h"

namespace Ui {
class InstalledItemWidget;
}

class InstalledItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstalledItemWidget(QWidget *parent = nullptr);
    ~InstalledItemWidget();

    void setData(QModelIndex index);
    void setModel(ApplicationModel* model);

    void setRow(int row);

signals:
    void likedClicked(int row);
    void infoClicked(QModelIndex& index);
    void deleteClicked(int row);

private:
    Ui::InstalledItemWidget *ui;
    int mRow = -1;
    ApplicationModel *mModel;
    QModelIndex mIndex;
};

#endif // INSTALLEDITEMWIDGET_H
