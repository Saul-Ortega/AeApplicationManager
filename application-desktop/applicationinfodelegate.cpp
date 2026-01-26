#include "applicationinfodelegate.h"
#include <QDialog>
#include <QLabel>

ApplicationInfoDelegate::ApplicationInfoDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    this->setParent(parent);
}

void ApplicationInfoDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    /*
    * DEBIDO A QUE EL PARENT ES DE TIPO QOBJECT,
    * HAY QUE CASTEARLO A UN QWIDGET PARA QUE ESTÉ
    * ASOCIADO EL MODAL AL WIDGET DESEADO
    */
    QWidget* parentWidget = qobject_cast<QWidget*>(this->parent());
    QDialog* dialog = new QDialog(parentWidget);
    dialog->setFixedHeight(parentWidget->height() - 100);
    dialog->setFixedWidth(parentWidget->width() - 100);
    // dialog->setStyleSheet("background-color: orange");
    dialog->setModal(true);
    dialog->open();

    // QRect titleRect = QRect(option.rect.x(), option.rect.y(), dialog->width(), 50);
    // QColor titleColor = QColor(0x303030);
    // painter->fillRect(titleRect, titleColor);

    QLabel* title = new QLabel(dialog);
    QFont* font = new QFont();
    font->setBold(true);
    font->setPixelSize(17);
    title->setFont(*font);
    title->setText("Hola");

    painter->restore();
}

QSize ApplicationInfoDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //TODO: IMPLEMENTAR ESTA FUNCIÓN
    return QSize(0, 0);
}
