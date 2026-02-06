#ifndef APPLICATIONDELEGATE_H
#define APPLICATIONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include "applicationmodel.h"

class ApplicationDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ApplicationDelegate(QObject* parent = 0);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void isLikedButtonClicked(const QModelIndex& index);
    void infoButtonClicked(const QModelIndex& index);
    void isDownloadedButtonClicked(const QModelIndex& index);
    
public slots:
    void onMenuStyleClicked(const bool& isMenuStyle);

private:
    void paintProgressBar(QPainter* painter, const QRect& mainRectangle, const QRect& nameRectangle, const QModelIndex& index) const;

private:
    bool mIsMenuStyle;
    bool mIsButtonPressed;
    bool mIsButtonHover;
    bool mIsAtButtonsPosition;
};

#endif // APPLICATIONDELEGATE_H
