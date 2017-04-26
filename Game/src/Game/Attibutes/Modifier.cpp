#include <Engine/Debug/Logger.hpp>

#include <Game/Attibutes/Modifier.hpp>

Modifier::Modifier(double baseValue) : _value(baseValue)
{
}

Modifier::~Modifier()
{
}

double Modifier::getValue() const
{
    return this->_value;
}