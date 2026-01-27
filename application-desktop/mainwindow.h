#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <applicationmanagerwidget.h>
#include <applicationmodel.h>
#include <versionmodel.h>
#include "QItemSelectionModel"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayApplicationManager();

private:
    Ui::MainWindow *ui;
    ApplicationManagerWidget* mApplicationManagerWidget;
    QStackedWidget* mStackedWidget;
};
#endif // MAINWINDOW_H
