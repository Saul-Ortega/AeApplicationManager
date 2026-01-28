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

private:
    Ui::ApplicationInfoDialog *ui;
    ApplicationModel* mApplicationModel;
};

#endif // APPLICATIONINFODIALOG_H
