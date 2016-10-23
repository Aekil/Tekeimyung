#pragma once

#include "System.hpp"

class AISystem: public System
{
public:
    AISystem();
    virtual ~AISystem();
    virtual void update(EntityManager &em, float elapsedTime);
};
