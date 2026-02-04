#include "installerworker.h"
#include <QThread>

installerWorker::installerWorker(int appId, QObject* parent) :
    QObject(parent),
    mAppId(appId)
{
}

void installerWorker::install(){

    //EL BUCLE QUE SIMULA EL PROGRESO DE LA PROGRESSBAR
    for (int i=0; i<=100; i++){
        //PARAMOS EL HILO POR CADA VUELTA DEL FOR PARA QUE SE VEA EN LA UI
        QThread::msleep(50);
        //EMITIMOS EL ID Y EL PROGRESO QUE LLEVA
        emit progress(mAppId, i);
    }
    //ENVIAMOS EL ID DE LA APP QUE TERMINÓ
    emit finished(mAppId);
}
