#include <algorithm>

#include <Engine/Utils/Logger.hpp>

#include <Game/Attibutes/Attribute.hpp>

Attribute::Attribute(double startingValue)
{
    this->_baseValue = startingValue;
}

Attribute::~Attribute()
{
}

void Attribute::addBonus(Bonus bonus)
{
    this->_bonuses.push_back(bonus);
}

void Attribute::removeBonus(Bonus bonus)
{
    this->_bonuses.erase(std::remove(this->_bonuses.begin(), this->_bonuses.end(), bonus), this->_bonuses.end());
}

void Attribute::calculateValue()
{
    this->_finalValue = this->_baseValue;

    double bonusMultiplier = 0.0;

    for (auto bonus : this->_bonuses)
    {
        bonusMultiplier += bonus.getValue();
    }

    this->_finalValue *= (1.0f + bonusMultiplier);
}

double Attribute::getFinalValue()
{
    this->calculateValue();

    return this->_finalValue;
}