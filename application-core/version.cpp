#include "version.h"

//CONSTRUCTOR
Version::Version(const QString& name, const double& size, const time_t& lastModification)
    : mId(-1)
    , mName(name)
    , mSize(size)
    , mLastModification(lastModification)
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

time_t Version::lastModification() const
{
    return mLastModification;
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

void Version::setLastModification(const time_t& lastModification)
{
    mLastModification = lastModification;
}
