#include "applicationdelegate.h"

#include "applicationmodel.h"
#include <qpainterpath.h>
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
#include <QTimer>

ApplicationDelegate::ApplicationDelegate(QObject* parent) : QStyledItemDelegate(parent)
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

        //BOTÓN DE NOTIFICACIÓN
        // QPixmap bellPixmap = QPixmap(":/assets/campana.png");
        // bellPixmap = bellPixmap.scaled(QSize(15, 15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // QPoint bellPixmapPoint = QPoint(notificationRectangle.left() + (notificationRectangle.width() - bellPixmap.width()) / 2, notificationRectangle.top() + (notificationRectangle.height() - bellPixmap.height()) / 2);
        // painter->drawPixmap(bellPixmapPoint, bellPixmap);

        QStyleOptionButton notificationButton;
        notificationButton.rect = notificationButtonRectangle;
        notificationButton.icon = QIcon(":/assets/campana.png");
        notificationButton.iconSize = QSize(15, 15);
        notificationButton.state = QStyle::State_Enabled;

        QApplication::style()->drawControl(QStyle::CE_PushButton, &notificationButton, painter);
    }

    //CONTENEDOR QUE TENDRÁ LA IMÁGEN
    QSize imageRectangleSize = QSize(140, 70);
    QPoint imageRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - imageRectangleSize.width()) / 2, mainRectangle.top() + 25);
    QRect imageRectangle(imageRectanglePoint, imageRectangleSize);
    painter->drawRoundedRect(imageRectangle, borderRadius, borderRadius);

    //IMAGEN
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
    QStyleOptionButton likedButton;
    likedButton.rect = likedButtonRectangle;
    likedButton.icon = isLiked ? QIcon(":/assets/CorazonSeleccionado.png") : QIcon(":/assets/Corazon.png");
    likedButton.iconSize = QSize(25, 25);
    likedButton.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &likedButton, painter);

    //CONTENEDOR DE BOTÓN DE DETALLE O INFORMACIÓN
    int totalMargin = 10;
    qreal borderRadiusInfoButtonRectangle = 18;
    QSize infoButtonRectangleSize = QSize(buttonsRectangle.width() - (likedButtonRectangle.width() * 2) - totalMargin, 30);
    QPoint infoButtonRectanglePoint = QPoint(likedButtonRectangle.right() + (totalMargin / 2), buttonsRectangle.top());
    QRect infoButtonRectangle = QRect(infoButtonRectanglePoint, infoButtonRectangleSize);
    painter->drawRoundedRect(infoButtonRectangle, borderRadiusInfoButtonRectangle, borderRadiusInfoButtonRectangle);

    //BOTÓN DE DETALLE O INFORMACIÓN
    QStyleOptionButton infoButton;
    infoButton.rect = infoButtonRectangle;
    infoButton.icon = QPixmap(":/assets/Icon_Info.png");
    infoButton.iconSize = QSize(25, 25);
    infoButton.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &infoButton, painter);

    //CONTENEDOR DE BOTÓN DE INSTALADO
    QSize installedButtonRectangleSize = QSize(30, 30);
    QPoint installedButtonRectanglePoint = QPoint(buttonsRectangle.right() - installedButtonRectangleSize.width(), buttonsRectangle.top());
    QRect installedButtonRectangle = QRect(installedButtonRectanglePoint, installedButtonRectangleSize);
    painter->drawRoundedRect(installedButtonRectangle, borderRadiusCircle, borderRadiusCircle);

    //BOTÓN DE INSTALADO
    QStyleOptionButton installedButton;
    QStyleOptionProgressBar progressBar;

    QRect progressRect(imageRectangle.left(), imageRectangle.bottom() + 5, imageRectangle.width(), 8);
    int progress = mProgress.value(QPersistentModelIndex(index), -1);

    if (progress >= 0 && progress < 100) {

        QStyleOptionProgressBar progressBar;
        progressBar.rect = progressRect;
        progressBar.minimum = 0;
        progressBar.maximum = 100;
        progressBar.progress = progress;
        progressBar.textVisible = false;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBar, painter);
    }


    installedButton.rect = installedButtonRectangle;
    installedButton.icon = isDownloaded ? QIcon(":/assets/papelera.png") : QIcon(":/assets/Icon_Download.png");
    installedButton.iconSize = QSize(25, 25);
    installedButton.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &installedButton, painter);

    painter->restore();
}

QSize ApplicationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(180, 190);
}

bool ApplicationDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if ( event->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent* mouseEvent = (QMouseEvent *)event;

        QRect rect = option.rect;

        int x = rect.left() + 20;
        int y = rect.top() + 154;

        int notificationButtonWidth = 20;
        int likedButtonWidth = 30;
        int infoButtonWidth = 70;
        int installedButtonWidth = 30;
        int buttonsHeight = 40;

        QRect notificationButtonRectangle = QRect(QPoint(rect.right() - 5 - notificationButtonWidth, rect.top() + 5), QSize(notificationButtonWidth, notificationButtonWidth));
        QRect likedButtonRectangle = QRect(QPoint(x, y), QSize(likedButtonWidth, buttonsHeight));
        QRect infoButtonRectangle = QRect(QPoint(likedButtonRectangle.x() + likedButtonWidth + 5, y), QSize(infoButtonWidth, buttonsHeight));
        QRect installedButtonRectangle = QRect(QPoint(infoButtonRectangle.x() + infoButtonWidth + 5, y), QSize(installedButtonWidth, buttonsHeight));

        if ( notificationButtonRectangle.contains(mouseEvent->pos()) ) {
            bool isUpdated = model->data(index, ApplicationModel::UpdateRole).toBool();
            if ( !isUpdated ) {
                QToolTip::showText(QPoint(QCursor::pos().x() - 100, QCursor::pos().y()), "Tienes actualizaciones pendientes");
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

