#ifndef AVAILABLEITEMWIDGET_H
#define AVAILABLEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class AvailableItemWidget;
}

class AvailableItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableItemWidget(QWidget *parent = nullptr);
    ~AvailableItemWidget();

    void setData(const QString &name, const QString &imagePath);


    void setRow(int r);

signals:
    void infoClicked(int row);
    void favoriteClicked(int row);
    void deleteClicked(int row);

private slots:
    void onInfoButtonClicked();
    void onFavoriteButtonClicked();
    void onDeleteButtonClicked();

private:
    Ui::AvailableItemWidget *ui;
    int mRow = -1;
};

#endif // AVAILABLEITEMWIDGET_H
