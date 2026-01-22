#ifndef VERSION_H
#define VERSION_H

#include "application-core_global.h"
#include <QDate>

class APPLICATION_CORE_EXPORT Version
{
public:
    explicit Version(const QString& name = "", const double& size = 0, const QDate& lastModification = QDate(), const QDate& expirationDate = QDate(), const bool& isInstalled = false);

    int id() const;
    QString name() const;
    double size() const;
    QDate lastModification() const;
    QDate expirationDate() const;
    bool isInstalled() const;

    void setId(int id);
    void setName(const QString& name);
    void setSize(const double& size);
    void setLastModification(const QDate& lastModification);
    void setExpirationDate(const QDate& expirationDate);
    void setIsInstalled(const bool& isInstalled);

private:
    int mId;
    QString mName;
    double mSize;
    QDate mLastModification;
    QDate mExpirationDate;
    bool mIsInstalled;
};

#endif // VERSION_H
