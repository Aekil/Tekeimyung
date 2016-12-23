/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define COLLISION_SYSTEM_NAME "Collision system"

START_SYSTEM(CollisionSystem)
public:
    CollisionSystem();
    virtual ~CollisionSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    bool            isColliding(Entity *firstEntity, Entity *secondEntity);

    ADD_MONITORING_VAR
END_SYSTEM(CollisionSystem)
