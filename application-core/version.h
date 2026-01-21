#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QJsonObject>

class Version
{
public:
    int id=0;
    QString name="";
    double size=0.0;
    QString lastModification="";
    QString expirationDate="";
    bool is_installed;
    };

#endif // VERSION_H
