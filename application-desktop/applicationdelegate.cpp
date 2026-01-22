#include "applicationdelegate.h"
#include <QPainter>

const int ITEM_SIZE = 120;
const int TEXT_HEIGHT = 20;

ApplicationDelegate::ApplicationDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ApplicationDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    painter->save();

    // FONDO DEL ICONO
    painter->fillRect(option.rect, QColor("#f0f0f0"));

    // OBTENER IMAGEN
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();

    // CALCULAR EL CENTRO
    //el espacio que sobra /2 para encontrar el centro
    int imgX = option.rect.x() + (ITEM_SIZE - pixmap.width()) / 2;
    int imgY = option.rect.y() + (ITEM_SIZE - TEXT_HEIGHT - pixmap.height()) / 2;

    // IMAGEN CENTRADA
    painter->drawPixmap(imgX, imgY, pixmap);

    // NOMBRE DE LA APP
    QString name = index.data(Qt::DisplayRole).toString();

    //RECTANGULO DEL NOMBRE
    QRect textRect(option.rect.x(), option.rect.y() + ITEM_SIZE - TEXT_HEIGHT,
                   ITEM_SIZE,
                   TEXT_HEIGHT);

    painter->setPen(Qt::black);
    painter->drawText(textRect, Qt::AlignCenter, name);

    painter->restore();
}

QSize ApplicationDelegate::sizeHint(const QStyleOptionViewItem &,
                                    const QModelIndex &) const
{
    return QSize(ITEM_SIZE, ITEM_SIZE);
}
