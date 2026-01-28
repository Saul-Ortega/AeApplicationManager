#ifndef AVAILABLEAPPLICATIONSWIDGET_H
#define AVAILABLEAPPLICATIONSWIDGET_H

#include <QWidget>
#include "applicationmodel.h"
#include "versionmodel.h"
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

    void onDisponiblesClicked();
    void onDeseadosClicked();

private:
    Ui::AvailableApplicationsWidget *ui;
    ApplicationModel *mModel;
    bool mMostrarDeseados = false;
};

#endif // AVAILABLEAPPLICATIONSWIDGET_H
