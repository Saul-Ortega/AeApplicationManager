#ifndef APPLICATIONDAO_H
#define APPLICATIONDAO_H

#include <QString>
#include <QList>
#include "application.h"
#include "application-core_global.h"

class APPLICATION_CORE_EXPORT ApplicationDao
{
public:
    //CONSTRUCTOR -> RUTA DEL ARCHIVO
    explicit ApplicationDao();
    //ACTUALIZA UNA APLICACIÓN
    void updateApplication(const Application& application) const;
    //CARGA TODAS LAS APLICACIONES DEL JSON
    std::unique_ptr<std::vector<std::unique_ptr<Application>>> applications() const;
};

#endif // APPLICATIONDAO_H
