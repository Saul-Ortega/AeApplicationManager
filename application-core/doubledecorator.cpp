#include "doubledecorator.h"

#include <QVariant>

class DoubleDecorator::Implementation
{
public:
    Implementation(DoubleDecorator* _doubleDecorator, const double _value)
        : doubleDecorator(_doubleDecorator)
        , value(_value)
    {
    }

    DoubleDecorator* doubleDecorator{nullptr};
    double value;
};

DoubleDecorator::DoubleDecorator(Entity* parentEntity, const QString& key, const QString& label, const double& value)
    : DataDecorator(parentEntity, key, label)
{
    implementation.reset(new Implementation(this, value));
}

DoubleDecorator::~DoubleDecorator()
{
}

const double& DoubleDecorator::value() const
{
    return implementation->value;
}

DoubleDecorator& DoubleDecorator::setValue(const double& value)
{
    if ( value != implementation->value ) {
        implementation->value = value;
        emit valueChanged();
    }
    return *this;
}

QJsonValue DoubleDecorator::jsonValue() const
{
    return QJsonValue::fromVariant(QVariant(implementation->value));
}

void DoubleDecorator::update(const QJsonObject& _jsonObject)
{
    if ( _jsonObject.contains(key()) ) {
        setValue(_jsonObject.value(key()).toDouble());
    }
}
