/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <vector>

#include <Game/Attibutes/Bonus.hpp>

class Attribute
{
public:
    Attribute(double);
    ~Attribute();
    void addBonus(Bonus);
    void removeBonus(Bonus);
    void calculateValue();
    double getFinalValue();
public:

private:
    double _baseValue;
    std::vector<Bonus> _bonuses;
    double _finalValue;
};