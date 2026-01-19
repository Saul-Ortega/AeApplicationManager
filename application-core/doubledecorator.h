#ifndef DOUBLEDECORATOR_H
#define DOUBLEDECORATOR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QScopedPointer>

#include <application-core_global.h>
#include "datadecorator.h"

class APPLICATION_CORE_EXPORT DoubleDecorator : public DataDecorator
{
    Q_OBJECT
    Q_PROPERTY( double ui_value READ value WRITE setValue NOTIFY valueChanged )

public:
    DoubleDecorator(Entity* parentEntity = nullptr, const QString& key = "", const QString& label = "", const double& value = 0);
    ~DoubleDecorator();

    DoubleDecorator& setValue(const double& value);
    const double& value() const;

    QJsonValue jsonValue() const override;
    void update(const QJsonObject& jsonObject) override;

signals:
    void valueChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};

#endif // DOUBLEDECORATOR_H
