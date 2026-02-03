#include "searchwidget.h"
#include "ui_searchwidget.h"
#include <QDialog>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchWidget)
    , mIsMenuStyle(false)
{
    ui->setupUi(this);

    QPixmap icon(":/assets/lupa.png");
    int width = ui->icon->width();
    int height = ui->icon->height();
    ui->icon->setPixmap(icon.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon->setAlignment(Qt::AlignCenter);

    //CUANDO CAMBIA ALGO DEL INPUT MANDA UNA SEÑAL CON EL TEXTO
    connect(ui->input, &QLineEdit::textChanged, this, &SearchWidget::searchText);
    connect(ui->viewStyle, &QPushButton::clicked, this, [this] () {
        mIsMenuStyle = !mIsMenuStyle;
        ui->viewStyle->setIcon(mIsMenuStyle ? QIcon(":/assets/grid.png") : QIcon(":/assets/menu.png"));
        emit changeToMenuStyle(mIsMenuStyle);
    });
}

SearchWidget::~SearchWidget()
{
    delete ui;
}
