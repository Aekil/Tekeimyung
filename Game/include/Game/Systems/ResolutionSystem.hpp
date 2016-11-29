#pragma once

#include <ECS/System.hpp>

class ResolutionSystem : public System
{
public:
    ResolutionSystem();
    virtual ~ResolutionSystem();
    virtual void update(EntityManager &em, float elapsedTime);
};
