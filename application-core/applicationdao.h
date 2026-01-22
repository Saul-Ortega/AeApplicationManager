#ifndef APPLICATIONDAO_H
#define APPLICATIONDAO_H

#include <QString>
#include <QList>
#include "application.h"
#include "application-core_global.h"

class APPLICATION_CORE_EXPORT ApplicationDao
{
public:
    //=== CONSTRUCTOR === ruta del archivo JSON
    explicit ApplicationDao(const QString& filePath);

    //carga en la lista todas las aplicaciones del JSON
    QList <Application>loadApplications();

    //guarda todos los cambios en el JSON
    void extracted(const QList<Application> &apps);
    // === SAVE ALL ===
    void saveAll(const QList<Application> &apps);
    
private:
    QString mFilePath; //ruta del JSON
};

#endif // APPLICATIONDAO_H
