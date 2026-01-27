#ifndef AVAILABLEITEMWIDGET_H
#define AVAILABLEITEMWIDGET_H

#include <QWidget>
#include <QItemSelection>
#include "applicationmodel.h"

namespace Ui {
class AvailableItemWidget;
}

class AvailableItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableItemWidget(const QItemSelection& selection, QWidget *parent = nullptr);
    explicit AvailableItemWidget(QWidget *parent = nullptr);
    ~AvailableItemWidget();

    void setData(QModelIndex index);
    void setModel(ApplicationModel* model);


    void setRow(int r);

signals:
    void downloadClicked(int row);
    void favoriteClicked(int row);
    void infoClicked(int row);

private slots:
    void onDownloadButtonClicked();
    void onFavoriteButtonClicked();
    void onInfoClicked();

private:
    Ui::AvailableItemWidget *ui;
    int mRow = -1;
    ApplicationModel *mModel;
    QItemSelection mSelection;
};

#endif // AVAILABLEITEMWIDGET_H
