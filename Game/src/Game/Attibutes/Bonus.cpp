#include <Engine/Utils/Logger.hpp>

#include <Game/Attibutes/Bonus.hpp>

Bonus::Bonus(double baseValue) : _value(baseValue)
{
}

Bonus::~Bonus()
{
}

double Bonus::getValue() const
{
    return this->_value;
}