/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(ResolutionSystem)
public:
    ResolutionSystem();
    virtual                                     ~ResolutionSystem();
    virtual void                                update(EntityManager &em, float elapsedTime);
END_SYSTEM(RenderingSystem)
