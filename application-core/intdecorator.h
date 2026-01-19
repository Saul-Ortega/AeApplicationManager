#ifndef INTDECORATOR_H
#define INTDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>

#include <application-core_global.h>
#include "datadecorator.h"

class APPLICATION_CORE_EXPORT IntDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY( int ui_value READ value WRITE setValue NOTIFY valueChanged )

public:
    IntDecorator(Entity* parentEntity = nullptr, const QString& key = "", const QString& label = "", const int& value = 0);
    ~IntDecorator();

    IntDecorator& setValue(const int& value);
    const int& value() const;

    QJsonValue jsonValue() const override;
    void update(const QJsonObject& jsonObject) override;

signals:
    void valueChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

#endif // INTDECORATOR_H
