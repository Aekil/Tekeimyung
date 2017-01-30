/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(MouseSystem)
public:
    MouseSystem();
    virtual ~MouseSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void hoverEntity(EntityManager&);

    Entity* previousEntity = nullptr;
END_SYSTEM(MouseSystem)
