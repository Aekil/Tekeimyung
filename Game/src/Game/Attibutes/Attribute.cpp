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

void Attribute::addModifier(Modifier modifier)
{
    this->_modifiers.push_back(modifier);
}

void Attribute::removeModifier(Modifier modifier)
{
    this->_modifiers.erase(std::remove(this->_modifiers.begin(), this->_modifiers.end(), modifier), this->_modifiers.end());
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

    int bonusTotal = 0;

    for (auto bonus : this->_bonuses)
    {
        bonusTotal += bonus.getValue();
    }

    double modifierMultiplier = 0.0;

    for (auto modifier : this->_modifiers)
    {
        modifierMultiplier += modifier.getValue();
    }

    this->_finalValue += bonusTotal;
    this->_finalValue *= (1.0f + modifierMultiplier);
}

double Attribute::getFinalValue()
{
    this->calculateValue();

    return this->_finalValue;
}

void Attribute::clearAll()
{
    this->clearAllBonuses();
    this->clearAllModifiers();
}

void Attribute::clearAllBonuses()
{
    this->_bonuses.clear();
}

void Attribute::clearAllModifiers()
{
    this->_modifiers.clear();
}

double Attribute::getBaseValue()
{
    return this->_baseValue;
}

void Attribute::setBaseValue(double baseValue)
{
    this->_baseValue = baseValue;
}