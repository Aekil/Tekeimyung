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
    virtual double getAttributes(std::string attributeName)
    {
        return this->_attributes[attributeName]->getFinalValue();
    };

protected:
    std::map<std::string, Attribute*> _attributes;
};