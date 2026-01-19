#ifndef DATADECORATOR_H
#define DATADECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>

#include <application-core_global.h>

class Entity;

class APPLICATION_CORE_EXPORT DataDecorator : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString ui_label READ label CONSTANT )
public:
    DataDecorator(Entity* parent = nullptr, const QString& key = "", const QString& label = "");
    virtual ~DataDecorator();

    const QString& key() const;
    const QString& label() const;
    Entity* parentEntity();

    virtual QJsonValue jsonValue() const = 0;
    virtual void update(const QJsonObject& jsonObject) = 0;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

#endif // DATADECORATOR_H
