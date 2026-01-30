#ifndef AVAILABLEAPPLICATIONSWIDGET_H
#define AVAILABLEAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include <QVector>
#include <QSortFilterProxyModel>
#include "filterproxymodel.h"

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
    void setApplicationModel(ApplicationModel* model);

signals:
    void infoClicked(const QModelIndex& index);

public slots:
    void onSearchText(const QString& text);


private slots:
    void onFavoriteClicked(int row);

    void onDisponiblesClicked();
    void onDeseadosClicked();

    void onDownloadClicked(int row);
    void onDownloadFinished(int row);

private:
    Ui::AvailableApplicationsWidget *ui;
    ApplicationModel *mModel;
    bool mMostrarDeseados = false;
    QMetaObject::Connection modelDataChangedConnect;

    FilterProxyModel* mProxyModel;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
