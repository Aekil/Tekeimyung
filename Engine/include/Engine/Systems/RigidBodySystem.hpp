/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(RigidBodySystem)
public:
    RigidBodySystem();
    virtual ~RigidBodySystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);
END_SYSTEM(RigidBodySystem)
