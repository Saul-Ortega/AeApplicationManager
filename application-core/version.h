#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include <QJsonObject>

class Version
{
public:
    int id{};
    QString name{};
    double size{};
    QString lastModification{};
    QString expirationDate{};
    };

#endif // VERSION_H
