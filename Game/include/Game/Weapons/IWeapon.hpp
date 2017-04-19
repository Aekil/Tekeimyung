/*
** Author : Simon AMBROISE
*/

#pragma once

#include <map>

#include <Engine/Components.hh>
#include <Engine/Graphics/Material.hpp>
#include <Game/Attibutes/Attribute.hpp>
#include <Game/Scripts/Player.hpp>

class   IWeapon
{
public:
    virtual void fire(Player* player, sTransformComponent* playerTransform, sRenderComponent* playerRender, glm::vec3 playerDirection) = 0;
    virtual void reload() = 0;
    virtual double getAttribute(std::string attributeName)
    {
        return this->_attributes[attributeName]->getFinalValue();
    };

    virtual void upgradeByLevel() = 0;

    virtual void levelUp()
    {
        this->_level++;
        LOG_DEBUG("Weapon level up : %d", this->_level);
        this->upgradeByLevel();
    }

    virtual std::string getName() = 0;

    Material* getMaterial()
    {
        return (_material);
    }

protected:
    std::map<std::string, Attribute*> _attributes;
    Material* _material = nullptr;

    int _level = 0;
};
