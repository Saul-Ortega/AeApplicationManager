#ifndef INSTALLERWORKER_H
#define INSTALLERWORKER_H
#include "application-core_global.h"
#include <QObject>
#include <QString>

class APPLICATION_CORE_EXPORT installerWorker : public QObject
{
    Q_OBJECT
public:
    explicit installerWorker(int appId, QObject* parent = nullptr);

public slots:
    void install();

signals:
    void progress(int, int);
    void finished(int);

private:
    int mAppId;
};

#endif // INSTALLERWORKER_H
