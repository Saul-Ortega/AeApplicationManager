#ifndef APPLICATIONINFODIALOG_H
#define APPLICATIONINFODIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QThread>
#include "applicationmodel.h"
#include "filterproxymodel.h"
#include "installerworker.h"

namespace Ui {
class ApplicationInfoDialog;
}

class ApplicationModel;
class QItemSelectionModel;

class ApplicationInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationInfoDialog(QWidget *parent = nullptr);
    ~ApplicationInfoDialog();
    void setApplicationModel(ApplicationModel* model);

    void loadApplication(const QModelIndex& index);
    void loadVersion(const Version& version);

public slots:
    void onInstallProgress(QModelIndex sourceIndex, int progress);
    void onInstallFinished(QModelIndex sourceIndex);

signals:
    void buttonClicked();

private:
    Ui::ApplicationInfoDialog *ui;
    ApplicationModel* mModel;
    QModelIndex mIndex;
    Version mVersion;
};

#endif // APPLICATIONINFODIALOG_H
