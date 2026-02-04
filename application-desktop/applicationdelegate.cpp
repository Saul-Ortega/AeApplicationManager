#include "applicationdelegate.h"

#include "applicationmodel.h"
#include <qpainterpath.h>
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
#include <QTimer>
#include <QListView>

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
    qreal borderRadiusInfoButtonRectangle = 18;

    //QRECTS
    QRect mainRectangle;
    QRect notificationButtonRectangle;
    QRect imageRectangle;
    QRect nameRectangle;
    QRect likedButtonRectangle;
    QRect infoButtonRectangle;
    QRect installedButtonRectangle;

    //QPIXMAPS Y QPIXMAPPOINTS
    QPixmap notificationButtonPixmap;
    QPoint notificationButtonPixmapPoint;
    QPixmap pixmap;
    QPoint pixmapPoint;
    QPixmap likedButtonPixmap;
    QPoint likedButtonPixmapPoint;
    QPixmap infoButtonPixmap;
    QPoint infoButtonPixmapPoint;
    QPixmap installedButtonPixmap;
    QPoint installedButtonPixmapPoint;

    //SE RECOGE LA POSICIÓN DEL CURSOR DEL USUARIO
    QPoint cursor = QCursor::pos();

    //SE DECLARA UN OBJETO DE TIPO QLISTVIEW PARA PODER MAPEAR LA POSICIÓN GLOBAL DEL CURSOR
    const auto widget = qobject_cast<QListView*>(option.styleObject);

    //SE TRADUCE EL QPOINT GLOBAL AL QPOINT CORRESPONDIENTE DEL WIDGET
    QPoint position = widget->viewport()->mapFromGlobal(cursor);


    //SI EL USUARIO QUIERE EL TIPO DE LISTA EN GRID
    if ( !mIsMenuStyle ) {
        //CONTENEDOR PRINCIPAL CON EL BORDE REDONDEADO
        mainRectangle = QRect(option.rect.topLeft(), QSize(180, 190));

        if ( !isUpdated ) {
            //CONTENEDOR QUE TENDRÁ LA NOTIFICACIÓN
            int margin = 5;
            QSize notificationButtonRectangleSize = QSize(20, 20);
            QPoint notificationButtonRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - notificationButtonRectangleSize.width()) - margin, mainRectangle.top() + margin);
            notificationButtonRectangle = QRect(notificationButtonRectanglePoint, notificationButtonRectangleSize);

            notificationButtonPixmap = QPixmap(":/assets/campana.png");
            notificationButtonPixmap = notificationButtonPixmap.scaled(QSize(15, 15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            notificationButtonPixmapPoint = QPoint(notificationButtonRectangle.left() + (notificationButtonRectangle.width() - notificationButtonPixmap.width()) / 2, notificationButtonRectangle.top() + (notificationButtonRectangle.height() - notificationButtonPixmap.height()) / 2);
        }

        //CONTENEDOR QUE TENDRÁ LA IMÁGEN
        QSize imageRectangleSize = QSize(140, 70);
        QPoint imageRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - imageRectangleSize.width()) / 2, mainRectangle.top() + 25);
        imageRectangle = QRect(imageRectanglePoint, imageRectangleSize);

        //IMÁGEN DE LA APLICACIÓN
        pixmap = QPixmap(imageUrl);
        pixmap = pixmap.scaled(QSize(60, 60), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmapPoint = QPoint(imageRectangle.left() + (imageRectangle.width() - pixmap.width()) / 2, imageRectangle.top() + (imageRectangle.height() - pixmap.height()) / 2);

        //NOMBRE DE APLICACIÓN
        QPoint nameRectanglePoint = QPoint(mainRectangle.left(), imageRectangle.bottom());
        nameRectangle = QRect(nameRectanglePoint, QSize(mainRectangle.width(), 40));

        //CONTENEDOR DE BOTONES
        QSize buttonsRectangleSize = QSize(140, 40);
        QPoint buttonsRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - buttonsRectangleSize.width()) / 2, nameRectangle.top() + 50);
        QRect buttonsRectangle(buttonsRectanglePoint, buttonsRectangleSize);

        //CONTENEDOR DE BOTÓN DE FAVORITOS
        QSize likedButtonRectangleSize = QSize(30, 30);
        QPoint likedButtonRectanglePoint = QPoint(buttonsRectangle.left(), buttonsRectangle.top());
        likedButtonRectangle = QRect(likedButtonRectanglePoint, likedButtonRectangleSize);

        //BOTÓN DE FAVORITOS
        likedButtonPixmap = QPixmap(isLiked ? ":/assets/CorazonSeleccionado.png" : ":/assets/Corazon.png");
        likedButtonPixmap = likedButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        likedButtonPixmapPoint = QPoint(likedButtonRectangle.left() + (likedButtonRectangle.width() - likedButtonPixmap.width()) / 2, likedButtonRectangle.top() + (likedButtonRectangle.height() - likedButtonPixmap.height()) / 2);

        //CONTENEDOR DE BOTÓN DE DETALLE O INFORMACIÓN
        int totalMargin = 10;
        QSize infoButtonRectangleSize = QSize(buttonsRectangle.width() - (likedButtonRectangle.width() * 2) - totalMargin, 30);
        QPoint infoButtonRectanglePoint = QPoint(likedButtonRectangle.right() + (totalMargin / 2), buttonsRectangle.top());
        infoButtonRectangle = QRect(infoButtonRectanglePoint, infoButtonRectangleSize);

        //BOTÓN DE DETALLE O INFORMACIÓN
        infoButtonPixmap = QPixmap(":/assets/Icon_Info.png");
        infoButtonPixmap = infoButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        infoButtonPixmapPoint = QPoint(infoButtonRectangle.left() + (infoButtonRectangle.width() - infoButtonPixmap.width()) / 2, infoButtonRectangle.top() + (infoButtonRectangle.height() - infoButtonPixmap.height()) / 2);

        //CONTENEDOR DE BOTÓN DE INSTALADO
        QSize installedButtonRectangleSize = QSize(30, 30);
        QPoint installedButtonRectanglePoint = QPoint(buttonsRectangle.right() - installedButtonRectangleSize.width(), buttonsRectangle.top());
        installedButtonRectangle = QRect(installedButtonRectanglePoint, installedButtonRectangleSize);

        //BOTÓN DE INSTALADO
        installedButtonPixmap = QPixmap(isDownloaded ? ":/assets/papelera.png" : ":/assets/Icon_Download.png");
        installedButtonPixmap = installedButtonPixmap.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        installedButtonPixmapPoint = QPoint(installedButtonRectangle.left() + (installedButtonRectangle.width() - installedButtonPixmap.height()) / 2, installedButtonRectangle.top() + (installedButtonRectangle.height() - installedButtonPixmap.height()) / 2);
    } else {
        //SI EL USUARIO QUIERE EL TIPO DE LISTA EN MENU
        //CONTENEDOR PRINCIPAL CON EL BORDE REDONDEADO
        mainRectangle = QRect(option.rect.topLeft(), QSize(option.rect.width() - 5, 60));

        if ( !isUpdated ) {
            //CONTENEDOR QUE TENDRÁ LA NOTIFICACIÓN
            int margin = 5;
            QSize notificationButtonRectangleSize = QSize(20, 20);
            QPoint notificationButtonRectanglePoint = QPoint(mainRectangle.left() + (mainRectangle.width() - notificationButtonRectangleSize.width()) - margin, mainRectangle.top() + margin);
            notificationButtonRectangle = QRect(notificationButtonRectanglePoint, notificationButtonRectangleSize);

            notificationButtonPixmap = QPixmap(":/assets/campana.png");
            notificationButtonPixmap = notificationButtonPixmap.scaled(QSize(15, 15), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            notificationButtonPixmapPoint = QPoint(notificationButtonRectangle.left() + (notificationButtonRectangle.width() - notificationButtonPixmap.width()) / 2, notificationButtonRectangle.top() + (notificationButtonRectangle.height() - notificationButtonPixmap.height()) / 2);
        }

        //CONTENEDOR QUE TENDRÁ LA IMÁGEN
        QSize imageRectangleSize = QSize(180, 50);
        QPoint imageRectanglePoint = QPoint(mainRectangle.left() + 30, mainRectangle.top() + (mainRectangle.height() - imageRectangleSize.height()) / 2);
        imageRectangle = QRect(imageRectanglePoint, imageRectangleSize);

        //IMÁGEN DE LA APLICACIÓN
        pixmap = QPixmap(imageUrl);
        pixmap = pixmap.scaled(QSize(imageRectangle.width() - 10, imageRectangle.height() - 10), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmapPoint = QPoint(imageRectangle.left() + (imageRectangle.width() - pixmap.width()) / 2, imageRectangle.top() + (imageRectangle.height() - pixmap.height()) / 2);

        //NOMBRE DE LA APLICACIÓN
        QSize nameRectangleSize = QSize(150, 50);
        QPoint nameRectanglePoint = QPoint(imageRectangle.right() + 30, mainRectangle.top() + (mainRectangle.height() - nameRectangleSize.height()) / 2);
        nameRectangle = QRect(nameRectanglePoint, nameRectangleSize);

        //CONTENEDOR DE BOTONES
        QSize buttonsRectangleSize = QSize(300, mainRectangle.height());
        QPoint buttonsRectanglePoint = QPoint(mainRectangle.right() - 30 - buttonsRectangleSize.width(), mainRectangle.top() + (mainRectangle.height() - buttonsRectangleSize.height()) / 2);
        QRect buttonsRectangle(buttonsRectanglePoint, buttonsRectangleSize);

        //CONTENEDOR DE BOTÓN DE FAVORITOS
        QSize likedButtonRectangleSize = QSize(50, 50);
        QPoint likedButtonRectanglePoint = QPoint(buttonsRectangle.left(), buttonsRectangle.top() + (buttonsRectangleSize.height() - likedButtonRectangleSize.height()) / 2);
        likedButtonRectangle = QRect(likedButtonRectanglePoint, likedButtonRectangleSize);

        //BOTÓN DE FAVORITOS
        likedButtonPixmap = QPixmap(isLiked ? ":/assets/CorazonSeleccionado.png" : ":/assets/Corazon.png");
        likedButtonPixmap = likedButtonPixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        likedButtonPixmapPoint = QPoint(likedButtonRectangle.left() + (likedButtonRectangle.width() - likedButtonPixmap.width()) / 2, likedButtonRectangle.top() + (likedButtonRectangle.height() - likedButtonPixmap.height()) / 2);

        //CONTENEDOR DE BOTÓN DE DETALLE O INFORMACIÓN
        int totalMargin = 10;
        QSize infoButtonRectangleSize = QSize(buttonsRectangle.width() - (likedButtonRectangle.width() * 2) - totalMargin, 50);
        QPoint infoButtonRectanglePoint = QPoint(likedButtonRectangle.right() + (totalMargin / 2), buttonsRectangle.top() + (buttonsRectangleSize.height() - infoButtonRectangleSize.height()) / 2);
        infoButtonRectangle = QRect(infoButtonRectanglePoint, infoButtonRectangleSize);

        //BOTÓN DE DETALLE O INFORMACIÓN
        infoButtonPixmap = QPixmap(":/assets/Icon_Info.png");
        infoButtonPixmap = infoButtonPixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        infoButtonPixmapPoint = QPoint(infoButtonRectangle.left() + (infoButtonRectangle.width() - infoButtonPixmap.width()) / 2, infoButtonRectangle.top() + (infoButtonRectangle.height() - infoButtonPixmap.height()) / 2);

        //CONTENEDOR DE BOTÓN DE INSTALADO
        QSize installedButtonRectangleSize = QSize(50, 50);
        QPoint installedButtonRectanglePoint = QPoint(buttonsRectangle.right() - installedButtonRectangleSize.width(), buttonsRectangle.top() + (buttonsRectangleSize.height() - installedButtonRectangleSize.height()) / 2);
        installedButtonRectangle = QRect(installedButtonRectanglePoint, installedButtonRectangleSize);

        //BOTÓN DE INSTALADO
        installedButtonPixmap = QPixmap(isDownloaded ? ":/assets/papelera.png" : ":/assets/Icon_Download.png");
        installedButtonPixmap = installedButtonPixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        installedButtonPixmapPoint = QPoint(installedButtonRectangle.left() + (installedButtonRectangle.width() - installedButtonPixmap.height()) / 2, installedButtonRectangle.top() + (installedButtonRectangle.height() - installedButtonPixmap.height()) / 2);
    }

    QPen bluePen;
    bluePen.setBrush(QColor("#4fa0d8"));
    bluePen.setWidth(2);
    QPen blackPen;
    blackPen.setBrush(QColor("#000"));
    blackPen.setWidth(2);

    QBrush greyBackground = QBrush("#d1d1d1");
    QBrush lightBlueBackground = QBrush("#c5def2");

    QFont font;
    font.setPixelSize(14);
    painter->setFont(font);

    painter->setPen(bluePen);
    painter->setBrush(greyBackground);
    painter->drawRoundedRect(mainRectangle, borderRadius, borderRadius);
    painter->drawRoundedRect(notificationButtonRectangle, borderRadiusCircle, borderRadiusCircle);
    painter->drawPixmap(notificationButtonPixmapPoint, notificationButtonPixmap);
    painter->drawRoundedRect(imageRectangle, borderRadius, borderRadius);
    painter->drawPixmap(pixmapPoint, pixmap);
    painter->setPen(blackPen);
    painter->drawText(nameRectangle, Qt::AlignCenter, name);
    painter->setPen(bluePen);
    painter->drawRoundedRect(likedButtonRectangle, borderRadiusCircle, borderRadiusCircle);
    painter->drawPixmap(likedButtonPixmapPoint, likedButtonPixmap);
    painter->drawRoundedRect(infoButtonRectangle, borderRadiusInfoButtonRectangle, borderRadiusInfoButtonRectangle);
    painter->drawPixmap(infoButtonPixmapPoint, infoButtonPixmap);
    painter->drawRoundedRect(installedButtonRectangle, borderRadiusCircle, borderRadiusCircle);
    painter->drawPixmap(installedButtonPixmapPoint, installedButtonPixmap);
    paintProgressBar(painter, mainRectangle, nameRectangle, index);

    //HOVERS
    if ( notificationButtonRectangle.contains(position) ) {
        painter->setBrush(lightBlueBackground);
        painter->drawRoundedRect(notificationButtonRectangle, borderRadiusCircle, borderRadiusCircle);
        painter->drawPixmap(notificationButtonPixmapPoint, notificationButtonPixmap);
    }

    if ( likedButtonRectangle.contains(position) ) {
        painter->setBrush(lightBlueBackground);
        painter->drawRoundedRect(likedButtonRectangle, borderRadiusCircle, borderRadiusCircle);
        painter->drawPixmap(likedButtonPixmapPoint, likedButtonPixmap);
    }

    if ( infoButtonRectangle.contains(position) ) {
        painter->setBrush(lightBlueBackground);
        painter->drawRoundedRect(infoButtonRectangle, borderRadiusInfoButtonRectangle, borderRadiusInfoButtonRectangle);
        painter->drawPixmap(infoButtonPixmapPoint, infoButtonPixmap);
    }

    if ( installedButtonRectangle.contains(position) ) {
        painter->setBrush(lightBlueBackground);
        painter->drawRoundedRect(installedButtonRectangle, borderRadiusCircle, borderRadiusCircle);
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

    if ( event->type() == QEvent::MouseButtonRelease ) {
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

void ApplicationDelegate::paintProgressBar(QPainter* painter, const QRect& mainRectangle, const QRect& nameRectangle, const QModelIndex& index) const
{
    // SI EXISTE PROGRESO PARA ESTE INDICE DIBUJA LA BARRA
    if (mProgress.contains(index)) {

        // VALOR DEL PROGRESO (0–100)
        int progress = mProgress[index];

        // ALTURA DE LA LINEA
        int lineHeight = 4;

        // POSICIÓN VERTICAL
        int y = nameRectangle.bottom() + 0;

        // RECTANGULO DEL FONDO DE LA BARRA
        QRect progressBackground(
            mainRectangle.left() + 20,
            y,
            mainRectangle.width() - 40,
            lineHeight
            );

        // COLOR DEL FONDO
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#5e5e5e"));
        painter->drawRoundedRect(progressBackground, 2, 2);

        int filledWidth = (progressBackground.width() * progress) / 100;

        QRect progressFilled(progressBackground.left(),progressBackground.top(),filledWidth,lineHeight);

        // COLOR DE LA BARRA AZUL
        painter->setBrush(QColor("#4fa0d8"));
        painter->drawRoundedRect(progressFilled, 2, 2);
    }
}



