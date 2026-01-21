#include "application.h"

//CONSTRUCTOR
Application::Application(const QString& name, const QString& description, const QString& imageUrl, const QString& executableFile, const bool& isLiked, const bool& isDownloaded)
    : mId(-1)
    , mName(name)
    , mDescription(description)
    , mImageUrl(imageUrl)
    , mExecutableFile(executableFile)
    , mIsLiked(isLiked)
    , mIsDownloaded(isDownloaded)
{
}

//GETTERS
int Application::id() const
{
    return mId;
}

QString Application::name() const
{
    return mName;
}

QString Application::imageUrl() const
{
    return mImageUrl;
}

QString Application::executableFile() const
{
    return mExecutableFile;
}

bool Application::isLiked() const
{
    return mIsLiked;
}

bool Application::isDownloaded() const
{
    return mIsDownloaded;
}

//SETTERS
void Application::setId(int id)
{
    mId = id;
}

void Application::setName(const QString& name)
{
    mName = name;
}

void Application::setImageUrl(const QString& imageUrl)
{
    mImageUrl = imageUrl;
}

void Application::setExecutableFile(const QString& executableFile)
{
    mExecutableFile = executableFile;
}

void Application::setIsLiked(const bool& isLiked)
{
    mIsLiked = isLiked;
}

void Application::setIsDownloaded(const bool& isDownloaded)
{
    mIsDownloaded = isDownloaded;
}
