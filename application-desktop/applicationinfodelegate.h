#ifndef APPLICATIONINFODELEGATE_H
#define APPLICATIONINFODELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include "applicationmodel.h"

class ApplicationInfoDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ApplicationInfoDelegate(QObject* parent = 0);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // APPLICATIONINFODELEGATE_H
