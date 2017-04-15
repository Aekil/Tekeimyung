/*
** Author : Simon AMBROISE
*/

#pragma once

#include <map>

#include <Engine/Components.hh>

#include <Game/Attibutes/Attribute.hpp>

class IWeapon
{
public:
    virtual void fire(sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) = 0;
    virtual void reload() = 0;
    virtual double getAttribute(std::string attributeName)
    {
        return this->_attributes[attributeName]->getFinalValue();
    };

    virtual void upgradeByLevel() = 0;

    virtual void levelUp()
    {
        LOG_DEBUG("Weapon level up");
        this->_level++;
        this->upgradeByLevel();
    }

protected:
    std::map<std::string, Attribute*> _attributes;

    int _level = 0;
};