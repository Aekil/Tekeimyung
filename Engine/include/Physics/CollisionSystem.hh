#pragma once

#include "System.hpp"

class CollisionSystem : public System
{
public:
    CollisionSystem();
    virtual ~CollisionSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    bool            isColliding(Entity *firstEntity, Entity *secondEntity);
};