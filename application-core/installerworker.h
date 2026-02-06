#ifndef INSTALLERWORKER_H
#define INSTALLERWORKER_H
#include "application-core_global.h"
#include <QObject>
#include <QString>
#include <QModelIndex>

class APPLICATION_CORE_EXPORT installerWorker : public QObject
{
    Q_OBJECT
public:
    explicit installerWorker(QModelIndex sourceIndex, QObject* parent = nullptr);

public slots:
    void install();

signals:
    void progress(QModelIndex, int);
    void finished(QModelIndex);

private:
    QModelIndex mSourceIndex;
};

#endif // INSTALLERWORKER_H
