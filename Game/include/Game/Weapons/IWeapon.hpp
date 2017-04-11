/*
** Author : Simon AMBROISE
*/

#pragma once

#include <map>

#include <Game/Attibutes/Attribute.hpp>

class IWeapon
{
public:
    virtual void fire() = 0;
    virtual void reload() = 0;
    virtual double getAttributes(std::string attributeName)
    {
        return this->_attributes[attributeName]->getFinalValue();
    };

private:
    std::map<std::string, Attribute*> _attributes;
};