/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <vector>

#include <Game/Attibutes/Modifier.hpp>
#include <Game/Attibutes/Bonus.hpp>

class Attribute
{
public:
    Attribute(double);
    ~Attribute();
    void addModifier(Modifier);
    void removeModifier(Modifier);
    void addBonus(Bonus);
    void removeBonus(Bonus);
    void clearAllBonuses();
    void clearAllModifiers();
    void clearAll();

    void calculateValue();
    double getFinalValue();

    double getBaseValue();
    void setBaseValue(double);
public:

private:
    double _baseValue;
    std::vector<Modifier> _modifiers;
    std::vector<Bonus> _bonuses;
    double _finalValue;
};