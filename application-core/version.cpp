#include "version.h"

//CONSTRUCTOR
Version::Version(const QString& name, const double& size, const QDate& lastModification, const QDate& expirationDate, const bool& isInstalled)
    : mId(-1)
    , mName(name)
    , mSize(size)
    , mLastModification(lastModification)
    , mExpirationDate(expirationDate)
    , mIsInstalled(isInstalled)
{
}

//GETTERS
int Version::id() const
{
    return mId;
}

QString Version::name() const
{
    return mName;
}

double Version::size() const
{
    return mSize;
}

QDate Version::lastModification() const
{
    return mLastModification;
}

QDate Version::expirationDate() const
{
    return mExpirationDate;
}

bool Version::isInstalled() const
{
    return mIsInstalled;
}

//SETTERS
void Version::setId(int id)
{
    mId = id;
}

void Version::setName(const QString& name)
{
    mName = name;
}

void Version::setSize(const double& size)
{
    mSize = size;
}

void Version::setLastModification(const QDate& lastModification)
{
    mLastModification = lastModification;
}

void Version::setExpirationDate(const QDate& expirationDate)
{
    mExpirationDate = expirationDate;
}

void Version::setIsInstalled(const bool& isInstalled)
{
    mIsInstalled = isInstalled;
}
