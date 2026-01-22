#include "applicationinfodelegate.h"

ApplicationInfoDelegate::ApplicationInfoDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void ApplicationInfoDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    int x = option.rect.width();

    painter->restore();
}

QSize ApplicationInfoDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //TODO: IMPLEMENTAR ESTA FUNCIÓN
    return QSize();
}
