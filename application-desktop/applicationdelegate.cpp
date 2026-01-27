#include "applicationdelegate.h"
#include "applicationmodel.h"
#include <QPainter>
#include <QPainterPath>

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

    //QUITAR LAS ESQUINAS DE LA APP
    QPainterPath path;
    path.addRoundedRect(option.rect, 10, 10);

    // FONDO DEL ICONO
    painter->fillPath(path, QColor("#A3A3A3"));


    // OBTENER RUTA DE LA IMAGEN DESDE EL ROL
    QString imagePath = index.data(ApplicationModel::ImageUrlRole).toString();
    QPixmap pixmap(imagePath);

    // IMAGEN CENTRADA
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // CALCULAR EL CENTRO
    int imgX = option.rect.x() + (ITEM_SIZE - pixmap.width()) / 2;
    int imgY = option.rect.y() + (ITEM_SIZE - TEXT_HEIGHT - pixmap.height()) / 2;

    //DIBUJAR LA IMAGEN
    painter->drawPixmap(imgX, imgY, pixmap);

    // NOMBRE DE LA APP
    QString name = index.data(Qt::DisplayRole).toString();

    //NOMBRE CENTRADO
    QRect textRect(option.rect.x(),
                   option.rect.y() + ITEM_SIZE - TEXT_HEIGHT,
                   ITEM_SIZE,
                   TEXT_HEIGHT);

    //COLOR E IMPLEMENTACION DEL NOMBRE
    painter->setPen(Qt::black);
    painter->drawText(textRect, Qt::AlignCenter, name);

    //LIMPIAMOS EL PAINTER
    painter->restore();
}

    //TAMAÑO DE CADA ITEM
QSize ApplicationDelegate::sizeHint(const QStyleOptionViewItem &,
                                    const QModelIndex &) const
{
    return QSize(ITEM_SIZE, ITEM_SIZE);
}
