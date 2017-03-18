/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

START_SYSTEM(CollisionSystem)
public:
    CollisionSystem();
    virtual ~CollisionSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    bool            isColliding(Entity *firstEntity, Entity *secondEntity);
END_SYSTEM(CollisionSystem)
