#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include "version.h"

class Application
{
public:
    Application();
    int id=0;
    QString name;
    QString description;
    bool isDownloaded=false;
    QString imageUrl;
    QString executableFile;
    QString expirationDate;
    bool isLiked=false;
    int idVersionInstalled;

    QList<Version> versions;

};

#endif // APPLICATION_H
