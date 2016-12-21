/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/Entity.hpp>

class BasicClass
{
public : 
    BasicClass(Entity* entity);
    ~BasicClass();
    virtual void Start() = 0;
    virtual void Update() = 0;

private:

    template<typename componentType>
    componentType*                  getComponent() const
    {
        return this->entity->getComponent<componentType>();
    }

    Entity *entity;
};