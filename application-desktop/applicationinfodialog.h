#ifndef APPLICATIONINFODIALOG_H
#define APPLICATIONINFODIALOG_H

#include <QDialog>
#include <QItemSelection>
#include "applicationmodel.h"
#include "versionmodel.h"

namespace Ui {
class ApplicationInfoDialog;
}

class ApplicationModel;
class VersionModel;
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
    void onDownloadVersionClicked(const QString& versionName);

private:
    Ui::ApplicationInfoDialog *ui;
    ApplicationModel* mApplicationModel;
    QModelIndex mIndex;
    Version mVersion;
};

#endif // APPLICATIONINFODIALOG_H
