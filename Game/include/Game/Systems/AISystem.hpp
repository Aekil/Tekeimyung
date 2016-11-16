#pragma once

#include <ECS/System.hpp>

class AISystem: public System
{
public:
    AISystem();
    virtual ~AISystem();
    virtual void update(EntityManager &em, float elapsedTime);
};
