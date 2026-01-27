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
    void setApplicationSelectionModel(QItemSelectionModel* selectionModel);
    void setVersionModel(VersionModel* model);
    void setVersionSelectionModel(QItemSelectionModel* selectionModel);

private slots:
    void loadApplication(const QItemSelection& selected);
    void loadVersion(const QItemSelection& selected);

private:
    Ui::ApplicationInfoDialog *ui;
    ApplicationModel* mApplicationModel;
    QItemSelectionModel* mApplicationSelectionModel;
    VersionModel* mVersionModel;
    QItemSelectionModel* mVersionSelectionModel;
};

#endif // APPLICATIONINFODIALOG_H
