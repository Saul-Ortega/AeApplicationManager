#include "installerworker.h"
#include <QThread>
#include <QRandomGenerator>

installerWorker::installerWorker(QModelIndex sourceIndex, QObject* parent) :
    QObject(parent),
    mSourceIndex(sourceIndex)
{
}

void installerWorker::install(){

    quint32 tiempo = QRandomGenerator::global()->bounded(50, 101);

    //EL BUCLE QUE SIMULA EL PROGRESO DE LA PROGRESSBAR
    for (int i=0; i<=100; i++){
        //PARAMOS EL HILO POR CADA VUELTA DEL FOR PARA QUE SE VEA EN LA UI
        QThread::msleep(tiempo);
        //EMITIMOS EL ID Y EL PROGRESO QUE LLEVA
        emit progress(mSourceIndex, i);
    }
    //ENVIAMOS EL ID DE LA APP QUE TERMINÓ
    emit finished(mSourceIndex);
}
