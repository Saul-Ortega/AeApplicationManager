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
    void infoClicked(QModelIndex& index);
    //PROGRESSBAR
    void downloadFinished(int row);


private slots:
    void onDownloadButtonClicked();
    void onFavoriteButtonClicked();
    //PROGRESSBAR
    void startProgress();
    void updateProgress();
    void onDownloadComplete();

private:
    Ui::AvailableItemWidget *ui;
    int mRow = -1;
    ApplicationModel *mModel;
    QModelIndex mIndex;
    //PROGRESSBAR
    QTimer* mProgressTimer;
    int mProgressValue = 0;
};

#endif // AVAILABLEITEMWIDGET_H
