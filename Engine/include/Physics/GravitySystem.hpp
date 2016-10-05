#pragma once

#include "System.hpp"

class GravitySystem : public System
{
public:
    GravitySystem();
    virtual ~GravitySystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);
};