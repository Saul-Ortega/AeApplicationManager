#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QList>
#include "version.h"
#include <QDate>
#include "application-core_global.h"

class APPLICATION_CORE_EXPORT Application
{
public:
    explicit Application(const QString& name = "", const QString& description = "", const QString& imageUrl = "", const QString& executableFile = "", const QDate& expirationDate = QDate(), const bool& isLiked = false, const bool& isDownloaded = false, const QList<Version>& versions = QList<Version>());

    int id() const;
    QString name() const;
    QString description() const;
    QString imageUrl() const;
    QString executableFile() const;
    QDate expirationDate() const;
    bool isLiked() const;
    bool isDownloaded() const;
    QList<Version> versions() const;

    void setId(int id);
    void setName(const QString& name);
    void setDescription(const QString& description);
    void setImageUrl(const QString& imageUrl);
    void setExecutableFile(const QString& executableFile);
    void setExpirationDate(const QDate& expirationDate);
    void setIsLiked(const bool& isLiked);
    void setIsDownloaded(const bool& isDownloaded);
    void setVersions(const QList<Version>& versions);

private:
    int mId;
    QString mName;
    QString mDescription;
    QString mImageUrl;
    QString mExecutableFile;
    QDate mExpirationDate;
    bool mIsLiked;
    bool mIsDownloaded;
    QList<Version> mVersions;
};

#endif // APPLICATION_H
