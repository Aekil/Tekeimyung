#pragma once

#include "System.hpp"

class MovementSystem : public System
{
public:
    MovementSystem();
    virtual ~MovementSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);
};