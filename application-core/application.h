#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include "application-core_global.h"

class APPLICATION_CORE_EXPORT Application
{
public:
    explicit Application(const QString& name = "", const QString& description = "", const QString& imageUrl = "", const QString& executableFile = "", const bool& isLiked = false, const bool& isDownloaded = false);

    int id() const;
    QString name() const;
    QString imageUrl() const;
    QString executableFile() const;
    bool isLiked() const;
    bool isDownloaded() const;

    void setId(int id);
    void setName(const QString& name);
    void setImageUrl(const QString& imageUrl);
    void setExecutableFile(const QString& executableFile);
    void setIsLiked(const bool& isLiked);
    void setIsDownloaded(const bool& isDownloaded);

private:
    int mId;
    QString mName;
    QString mDescription;
    QString mImageUrl;
    QString mExecutableFile;
    bool mIsLiked;
    bool mIsDownloaded;
};

#endif // APPLICATION_H
