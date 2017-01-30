/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(GravitySystem)
public:
    GravitySystem();
    virtual ~GravitySystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);
END_SYSTEM(GravitySystem)
