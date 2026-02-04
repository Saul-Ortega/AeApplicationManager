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



private slots:
    void updateProgress();

signals:
    void isLikedButtonClicked(const QModelIndex& index);
    void infoButtonClicked(const QModelIndex& index);
    void isDownloadedButtonClicked(const QModelIndex& index);

    void progressUpdated();
    
public slots:
    void onMenuStyleClicked(const bool& isMenuStyle);

private:
    bool mIsMenuStyle;

    mutable QHash<QPersistentModelIndex, int> mProgress; //PROGRESO DE CADA FILA
    mutable QHash<QTimer*, QPersistentModelIndex> mTimerIndex; //TIMER DE CADA FILA
    void paintProgressBar(QPainter* painter, const QRect& mainRectangle, const QRect& nameRectangle, const QModelIndex& index) const;


};

#endif // APPLICATIONDELEGATE_H
