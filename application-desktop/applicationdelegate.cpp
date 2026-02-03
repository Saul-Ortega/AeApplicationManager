#include "applicationdelegate.h"

#include "applicationmodel.h"
#include <qpainterpath.h>
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
#include <QTimer>

ApplicationDelegate::ApplicationDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , mIsMenuStyle(false)
{
}

void ApplicationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    //ANTIALIASING ES UNA TÉCNICA PARA PRODUCIR BORDES MÁS SUAVES Y QUE NO SALGAN PIXELADOS
    painter->setRenderHints(QPainter::Antialiasing);

    //DATOS EXTRAÍDOS DEL MODELO
    QString imageUrl = index.model()->data(index, ApplicationModel::ImageUrlRole).toString();
    QString name = index.model()->data(index, ApplicationModel::NameRole).toString();
    bool isDownloaded = index.model()->data(index, ApplicationModel::IsDownloadedRole).toBool();
    bool isLiked = index.model()->data(index, ApplicationModel::IsLikedRole).toBool();
    bool isUpdated = index.model()->data(index, ApplicationModel::UpdateRole).toBool();

    //VARIABLES COMUNES
    qreal borderRadius = 5;
    qreal borderRadiusCircle = 100;

    //SI EL USUARIO QUIERE EL TIPO DE LISTA EN GRID
    if ( !mIsMenuStyle ) {
        //CONTENEDOR PRINCIPAL CON EL BORDE REDONDEADO
        QRect mainRectangle(option.rect.topLeft(), QSize(180, 190));
        painter->drawRoundedRect(mainRectangle, borderRadius, borderRadius);

        if ( !isUpdated ) {
            //CONTENEDOR QUE TENDRÁ LA NOTIFICACIÓN
            int margin = 5;
            QSize notificationButtonRectangleSize = QSize(20, 20);
            QPoint notificationButtonRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - notificationButtonRectangleSize.width()) - margin, mainRectangle.top() + margin);
            QRect notificationButtonRectangle(notificationButtonRectanglePoint, notificationButtonRectangleSize);
            painter->drawRoundedRect(notificationButtonRectangle, borderRadiusCircle, borderRadiusCircle);

            QPixmap notificationButtonPixmap(":/assets/campana.png");
            notificationButtonPixmap = notificationButtonPixmap.scaled(QSize(15, 15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint notificationButtonPixmapPoint = QPoint(notificationButtonRectangle.left() + (notificationButtonRectangle.width() - notificationButtonPixmap.width()) / 2, notificationButtonRectangle.top() + (notificationButtonRectangle.height() - notificationButtonPixmap.height()) / 2);
            painter->drawPixmap(notificationButtonPixmapPoint, notificationButtonPixmap);
        }

        //CONTENEDOR QUE TENDRÁ LA IMÁGEN
        QSize imageRectangleSize = QSize(140, 70);
        QPoint imageRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - imageRectangleSize.width()) / 2, mainRectangle.top() + 25);
        QRect imageRectangle(imageRectanglePoint, imageRectangleSize);
        painter->drawRoundedRect(imageRectangle, borderRadius, borderRadius);

        //IMÁGEN DE LA APLICACIÓN
        QPixmap pixmap = QPixmap(imageUrl);
        pixmap = pixmap.scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint pixmapPoint = QPoint(imageRectangle.left() + (imageRectangle.width() - pixmap.width()) / 2, imageRectangle.top() + (imageRectangle.height() - pixmap.height()) / 2);
        painter->drawPixmap(pixmapPoint, pixmap);

        //NOMBRE DE APLICACIÓN
        QPoint nameRectanglePoint = QPoint(mainRectangle.left(), imageRectangle.bottom());
        QRect nameRectangle(nameRectanglePoint, QSize(mainRectangle.width(), 40));
        painter->drawText(nameRectangle, Qt::AlignCenter, name);

        //CONTENEDOR DE BOTONES
        QSize buttonsRectangleSize = QSize(140, 40);
        QPoint buttonsRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - buttonsRectangleSize.width()) / 2, nameRectangle.top() + 50);
        QRect buttonsRectangle(buttonsRectanglePoint, buttonsRectangleSize);

        //CONTENEDOR DE BOTÓN DE FAVORITOS
        QSize likedButtonRectangleSize = QSize(30, 30);
        QPoint likedButtonRectanglePoint = QPoint(buttonsRectangle.left(), buttonsRectangle.top());
        QRect likedButtonRectangle = QRect(likedButtonRectanglePoint, likedButtonRectangleSize);
        painter->drawRoundedRect(likedButtonRectangle, borderRadiusCircle, borderRadiusCircle);

        //BOTÓN DE FAVORITOS
        QPixmap likedButtonPixmap(isLiked ? ":/assets/CorazonSeleccionado.png" : ":/assets/Corazon.png");
        likedButtonPixmap = likedButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint likedButtonPixmapPoint = QPoint(likedButtonRectangle.left() + (likedButtonRectangle.width() - likedButtonPixmap.width()) / 2, likedButtonRectangle.top() + (likedButtonRectangle.height() - likedButtonPixmap.height()) / 2);
        painter->drawPixmap(likedButtonPixmapPoint, likedButtonPixmap);

        //CONTENEDOR DE BOTÓN DE DETALLE O INFORMACIÓN
        int totalMargin = 10;
        qreal borderRadiusInfoButtonRectangle = 18;
        QSize infoButtonRectangleSize = QSize(buttonsRectangle.width() - (likedButtonRectangle.width() * 2) - totalMargin, 30);
        QPoint infoButtonRectanglePoint = QPoint(likedButtonRectangle.right() + (totalMargin / 2), buttonsRectangle.top());
        QRect infoButtonRectangle = QRect(infoButtonRectanglePoint, infoButtonRectangleSize);
        painter->drawRoundedRect(infoButtonRectangle, borderRadiusInfoButtonRectangle, borderRadiusInfoButtonRectangle);

        //BOTÓN DE DETALLE O INFORMACIÓN
        QPixmap infoButtonPixmap(":/assets/Icon_Info.png");
        infoButtonPixmap = infoButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint infoButtonPixmapPoint = QPoint(infoButtonRectangle.left() + (infoButtonRectangle.width() - infoButtonPixmap.width()) / 2, infoButtonRectangle.top() + (infoButtonRectangle.height() - infoButtonPixmap.height()) / 2);
        painter->drawPixmap(infoButtonPixmapPoint, infoButtonPixmap);

        //CONTENEDOR DE BOTÓN DE INSTALADO
        QSize installedButtonRectangleSize = QSize(30, 30);
        QPoint installedButtonRectanglePoint = QPoint(buttonsRectangle.right() - installedButtonRectangleSize.width(), buttonsRectangle.top());
        QRect installedButtonRectangle = QRect(installedButtonRectanglePoint, installedButtonRectangleSize);
        painter->drawRoundedRect(installedButtonRectangle, borderRadiusCircle, borderRadiusCircle);

        //BOTÓN DE INSTALADO
        QPixmap installedButtonPixmap(isDownloaded ? ":/assets/papelera.png" : ":/assets/Icon_Download.png");
        installedButtonPixmap = installedButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint installedButtonPixmapPoint = QPoint(installedButtonRectangle.left() + (installedButtonRectangle.width() - installedButtonPixmap.height()) / 2, installedButtonRectangle.top() + (installedButtonRectangle.height() - installedButtonPixmap.height()) / 2);
        painter->drawPixmap(installedButtonPixmapPoint, installedButtonPixmap);
    } else {
        //SI EL USUARIO QUIERE EL TIPO DE LISTA EN MENU
        //CONTENEDOR PRINCIPAL CON EL BORDE REDONDEADO
        QRect mainRectangle(option.rect.topLeft(), QSize(option.rect.width() - 5, 60));
        painter->drawRoundedRect(mainRectangle, borderRadius, borderRadius);

        if ( !isUpdated ) {
            //CONTENEDOR QUE TENDRÁ LA NOTIFICACIÓN
            int margin = 5;
            QSize notificationButtonRectangleSize = QSize(20, 20);
            QPoint notificationButtonRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - notificationButtonRectangleSize.width()) - margin, mainRectangle.top() + margin);
            QRect notificationButtonRectangle(notificationButtonRectanglePoint, notificationButtonRectangleSize);
            painter->drawRoundedRect(notificationButtonRectangle, borderRadiusCircle, borderRadiusCircle);

            QPixmap notificationButtonPixmap(":/assets/campana.png");
            notificationButtonPixmap = notificationButtonPixmap.scaled(QSize(15, 15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint notificationButtonPixmapPoint = QPoint(notificationButtonRectangle.left() + (notificationButtonRectangle.width() - notificationButtonPixmap.width()) / 2, notificationButtonRectangle.top() + (notificationButtonRectangle.height() - notificationButtonPixmap.height()) / 2);
            painter->drawPixmap(notificationButtonPixmapPoint, notificationButtonPixmap);
        }

        //CONTENEDOR QUE TENDRÁ LA IMÁGEN
        QSize imageRectangleSize = QSize(180, 50);
        QPoint imageRectanglePoint = QPoint(mainRectangle.left() + 30, mainRectangle.top() + (mainRectangle.height() - imageRectangleSize.height()) / 2);
        QRect imageRectangle(imageRectanglePoint, imageRectangleSize);
        painter->drawRoundedRect(imageRectangle, borderRadius, borderRadius);

        //IMÁGEN DE LA APLICACIÓN
        QPixmap pixmap = QPixmap(imageUrl);
        pixmap = pixmap.scaled(QSize(imageRectangle.width() - 10, imageRectangle.height() - 10), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint pixmapPoint = QPoint(imageRectangle.left() + (imageRectangle.width() - pixmap.width()) / 2, imageRectangle.top() + (imageRectangle.height() - pixmap.height()) / 2);
        painter->drawPixmap(pixmapPoint, pixmap);

        //NOMBRE DE LA APLICACIÓN
        QSize nameRectangleSize = QSize(150, 50);
        QPoint nameRectanglePoint = QPoint(imageRectangle.right() + 30, mainRectangle.top() + (mainRectangle.height() - nameRectangleSize.height()) / 2);
        QRect nameRectangle(nameRectanglePoint, nameRectangleSize);
        painter->drawText(nameRectangle, Qt::AlignCenter, name);

        //CONTENEDOR DE BOTONES
        QSize buttonsRectangleSize = QSize(300, mainRectangle.height());
        QPoint buttonsRectanglePoint = QPoint(mainRectangle.right() - 30 - buttonsRectangleSize.width(), mainRectangle.top() + (mainRectangle.height() - buttonsRectangleSize.height()) / 2);
        QRect buttonsRectangle(buttonsRectanglePoint, buttonsRectangleSize);

        //CONTENEDOR DE BOTÓN DE FAVORITOS
        QSize likedButtonRectangleSize = QSize(50, 50);
        QPoint likedButtonRectanglePoint = QPoint(buttonsRectangle.left(), buttonsRectangle.top() + (buttonsRectangleSize.height() - likedButtonRectangleSize.height()) / 2);
        QRect likedButtonRectangle = QRect(likedButtonRectanglePoint, likedButtonRectangleSize);
        painter->drawRoundedRect(likedButtonRectangle, borderRadiusCircle, borderRadiusCircle);

        //BOTÓN DE FAVORITOS
        QPixmap likedButtonPixmap(isLiked ? ":/assets/CorazonSeleccionado.png" : ":/assets/Corazon.png");
        likedButtonPixmap = likedButtonPixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint likedButtonPixmapPoint = QPoint(likedButtonRectangle.left() + (likedButtonRectangle.width() - likedButtonPixmap.width()) / 2, likedButtonRectangle.top() + (likedButtonRectangle.height() - likedButtonPixmap.height()) / 2);
        painter->drawPixmap(likedButtonPixmapPoint, likedButtonPixmap);

        //CONTENEDOR DE BOTÓN DE DETALLE O INFORMACIÓN
        int totalMargin = 10;
        qreal borderRadiusInfoButtonRectangle = 18;
        QSize infoButtonRectangleSize = QSize(buttonsRectangle.width() - (likedButtonRectangle.width() * 2) - totalMargin, 50);
        QPoint infoButtonRectanglePoint = QPoint(likedButtonRectangle.right() + (totalMargin / 2), buttonsRectangle.top() + (buttonsRectangleSize.height() - infoButtonRectangleSize.height()) / 2);
        QRect infoButtonRectangle = QRect(infoButtonRectanglePoint, infoButtonRectangleSize);
        painter->drawRoundedRect(infoButtonRectangle, borderRadiusInfoButtonRectangle, borderRadiusInfoButtonRectangle);

        //BOTÓN DE DETALLE O INFORMACIÓN
        QPixmap infoButtonPixmap(":/assets/Icon_Info.png");
        infoButtonPixmap = infoButtonPixmap.scaled(QSize(45, 45), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint infoButtonPixmapPoint = QPoint(infoButtonRectangle.left() + (infoButtonRectangle.width() - infoButtonPixmap.width()) / 2, infoButtonRectangle.top() + (infoButtonRectangle.height() - infoButtonPixmap.height()) / 2);
        painter->drawPixmap(infoButtonPixmapPoint, infoButtonPixmap);

        //CONTENEDOR DE BOTÓN DE INSTALADO
        QSize installedButtonRectangleSize = QSize(50, 50);
        QPoint installedButtonRectanglePoint = QPoint(buttonsRectangle.right() - installedButtonRectangleSize.width(), buttonsRectangle.top() + (buttonsRectangleSize.height() - installedButtonRectangleSize.height()) / 2);
        QRect installedButtonRectangle = QRect(installedButtonRectanglePoint, installedButtonRectangleSize);
        painter->drawRoundedRect(installedButtonRectangle, borderRadiusCircle, borderRadiusCircle);

        //BOTÓN DE INSTALADO
        QPixmap installedButtonPixmap(isDownloaded ? ":/assets/papelera.png" : ":/assets/Icon_Download.png");
        installedButtonPixmap = installedButtonPixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint installedButtonPixmapPoint = QPoint(installedButtonRectangle.left() + (installedButtonRectangle.width() - installedButtonPixmap.height()) / 2, installedButtonRectangle.top() + (installedButtonRectangle.height() - installedButtonPixmap.height()) / 2);
        painter->drawPixmap(installedButtonPixmapPoint, installedButtonPixmap);
    }

    painter->restore();
}

QSize ApplicationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return mIsMenuStyle ? QSize(option.rect.width() - 5, 60) : QSize(180, 190);
}

bool ApplicationDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if ( event->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent* mouseEvent = (QMouseEvent *)event;

        QRect rect = option.rect;

        bool isUpdated = model->data(index, ApplicationModel::UpdateRole).toBool();

        int x;
        int y;

        int notificationButtonWidth = 20;
        int likedButtonWidth;
        int infoButtonWidth;
        int installedButtonWidth;
        int buttonsHeight;

        QRect notificationButtonRectangle = QRect(QPoint(rect.right() - 5 - notificationButtonWidth, rect.top() + 5), QSize(notificationButtonWidth, notificationButtonWidth));
        QRect likedButtonRectangle;
        QRect infoButtonRectangle;
        QRect installedButtonRectangle;

        if ( !mIsMenuStyle ) {
            x = rect.left() + 20;
            y = rect.top() + 154;

            likedButtonWidth = 30;
            infoButtonWidth = 70;
            installedButtonWidth = 30;
            buttonsHeight = 40;
        } else {
            x = rect.right() - 300 - 30;
            y = rect.top() + 5;

            likedButtonWidth = 50;
            infoButtonWidth = 170;
            installedButtonWidth = 50;
            buttonsHeight = 50;
        }

        likedButtonRectangle = QRect(QPoint(x, y), QSize(likedButtonWidth, buttonsHeight));
        infoButtonRectangle = QRect(QPoint(likedButtonRectangle.x() + likedButtonWidth + 5, y), QSize(infoButtonWidth, buttonsHeight));
        installedButtonRectangle = QRect(QPoint(infoButtonRectangle.x() + infoButtonWidth + 5, y), QSize(installedButtonWidth, buttonsHeight));

        //EMITE LAS SEÑALES CUANDO SE PULSA DENTRO DEL QRECT DE CADA UNO
        if ( notificationButtonRectangle.contains(mouseEvent->pos()) ) {
            if ( !isUpdated ) {
                QToolTip::showText(QPoint(QCursor::pos().x(), QCursor::pos().y()), "Tienes actualizaciones pendientes");
            }
        }

        if ( likedButtonRectangle.contains(mouseEvent->pos()) ) {
            emit isLikedButtonClicked(index);
        }

        if ( infoButtonRectangle.contains(mouseEvent->pos()) ) {
            emit infoButtonClicked(index);
        }

        if (installedButtonRectangle.contains(mouseEvent->pos())) {

            // GUARDARMOS EL INDEX PARA EMITIRLO CUANDO TERMINE LA BARRA
            QPersistentModelIndex persistentIndex(index);
            // INICIAR PROGRESO EN 0
            mProgress[persistentIndex] = 0;

            //CREAMOS QTIMER QUE LLAME A updateProgress cada 100ms
            QTimer *timer = new QTimer(this);
            mTimerIndex[timer] = persistentIndex;
            connect(timer, &QTimer::timeout, this, &ApplicationDelegate::updateProgress);
            timer->start(50);

            return true;
        }
    }

    return true;
}

//SEÑALES
void ApplicationDelegate::onMenuStyleClicked(const bool& isMenuStyle)
{
    mIsMenuStyle = isMenuStyle;
}

void ApplicationDelegate::updateProgress()
{
    QTimer *timer = qobject_cast<QTimer*>(sender());
    QPersistentModelIndex persistentIndex = mTimerIndex[timer];

    // INCREMENTA EL PROGRESO
    mProgress[persistentIndex] += 1;

    if (mProgress[persistentIndex] >= 100) {

        // LIMPIAMOS EL TIMER
        timer->stop();
        mTimerIndex.remove(timer);
        timer->deleteLater();

        // GUARDAMOS EL INDICE ANTES DE BORRAR EL PROGRESO
        QModelIndex finalIndex = QModelIndex(persistentIndex);

        // LIMPIAMOS EL PROGRESO
        mProgress.remove(persistentIndex);

        // EMITIMOS EL INDEX FINAL PARA CAMBIAR EL JSON
        emit isDownloadedButtonClicked(finalIndex);
    }

    // REPINTAMOS LA VISTA
    emit progressUpdated();
}

