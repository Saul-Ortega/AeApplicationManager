#ifndef VERSION_H
#define VERSION_H

#include <QString>
#include "application-core_global.h"
#include <ctime>

class APPLICATION_CORE_EXPORT Version
{
public:
    explicit Version(const QString& name = "", const double& size = 0, const time_t& lastModification = 0);

    int id() const;
    QString name() const;
    double size() const;
    time_t lastModification() const;

    void setId(int id);
    void setName(const QString& name);
    void setLastModification(const time_t& lastModification);

private:
    int mId;
    QString mName;
    double mSize;
    time_t mLastModification;
};

#endif // VERSION_H
