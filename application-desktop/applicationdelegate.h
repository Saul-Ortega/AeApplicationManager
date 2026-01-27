#ifndef APPLICATIONDELEGATE_H
#define APPLICATIONDELEGATE_H

#include <QStyledItemDelegate>

class ApplicationDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ApplicationDelegate(QObject *parent = nullptr);

    // DIBUJA CADA ITEM
    void paint(QPainter *painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    // TAMAÑO DEL CUADRADO DE LA APP
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

#endif // APPLICATIONDELEGATE_H
