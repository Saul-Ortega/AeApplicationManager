#include "searchwidget.h"
#include "ui_searchwidget.h"
#include "searchdelegate.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    QPixmap icon(":/assets/lupa.png");
    int width = ui->icon->width();
    int height = ui->icon->height();
    ui->icon->setPixmap(icon.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon->setAlignment(Qt::AlignCenter);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}
