#ifndef APPLICATIONMANAGERWIDGET_H
#define APPLICATIONMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class ApplicationManager;
}

class ApplicationManager : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationManager(QWidget *parent = nullptr);
    ~ApplicationManager();

private:
    Ui::ApplicationManager *ui;
};

#endif // APPLICATIONMANAGERWIDGET_H
