#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

signals:
    void searchText(const QString& text);
    void changeToMenuStyle(const bool& isMenuStyle);

private:
    Ui::SearchWidget *ui;
    bool mIsMenuStyle;
};

#endif // SEARCHWIDGET_H
