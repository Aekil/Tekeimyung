/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/Entity.hpp>
#include <ECS/System.hpp>

START_SYSTEM(MouseSystem)
public:
    MouseSystem();
    virtual ~MouseSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void hoverEntity(EntityManager&);

    Entity::sHandle previousEntityHandler = 0;
END_SYSTEM(MouseSystem)
