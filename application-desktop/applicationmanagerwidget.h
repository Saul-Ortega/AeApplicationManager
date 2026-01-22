#ifndef APPLICATIONMANAGERWIDGET_H
#define APPLICATIONMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class ApplicationManagerWidget;
}

class ApplicationManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationManagerWidget(QWidget *parent = nullptr);
    ~ApplicationManagerWidget();

private:
    Ui::ApplicationManagerWidget *ui;
};

#endif // APPLICATIONMANAGERWIDGET_H
