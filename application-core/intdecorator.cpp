#include "intdecorator.h"

#include <QVariant>

class IntDecorator::Implementation
{
public:
    Implementation(IntDecorator* _intDecorator, const int _value)
        : intDecorator(_intDecorator)
        , value(_value)
    {
    }

    IntDecorator* intDecorator{nullptr};
    int value;
};

IntDecorator::IntDecorator(Entity* parentEntity, const QString& key, const QString& label, const int& value)
    : DataDecorator(parentEntity, key, label)
{
    implementation.reset(new Implementation(this, value));
}

IntDecorator::~IntDecorator()
{
}

const int& IntDecorator::value() const
{
    return implementation->value;
}

IntDecorator& IntDecorator::setValue(const int& value)
{
    if ( value != implementation->value ) {
        implementation->value = value;
        emit valueChanged();
    }
    return *this;
}

QJsonValue IntDecorator::jsonValue() const
{
    return QJsonValue::fromVariant(QVariant(implementation->value));
}

void IntDecorator::update(const QJsonObject& _jsonObject)
{
    if ( _jsonObject.contains(key()) ) {
        setValue(_jsonObject.value(key()).toInt());
    }
}
