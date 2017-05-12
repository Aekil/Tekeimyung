/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>
#include <Engine/Core/Components/RigidBodyComponent.hh>

START_SYSTEM(RigidBodySystem)
public:
    RigidBodySystem();
    virtual ~RigidBodySystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void handleCollisions(EntityManager &em, Entity* entity, sRigidBodyComponent* rigidBody);
END_SYSTEM(RigidBodySystem)
