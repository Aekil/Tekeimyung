/*
** @Author : Simon AMBROISE
*/

#include <Engine/Debug/Logger.hpp>

#include <Game/Attibutes/Bonus.hpp>

Bonus::Bonus(int baseValue) : _value(baseValue)
{
}

Bonus::~Bonus()
{
}

int Bonus::getValue() const
{
    return this->_value;
}
