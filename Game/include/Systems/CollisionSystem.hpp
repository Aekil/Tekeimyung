#pragma once

#include "System.hpp"
#include "Core/Map.hpp"

class CollisionSystem : public System
{
public:
    CollisionSystem(Map* map);
    virtual ~CollisionSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    bool            isColliding(Entity *firstEntity, Entity *secondEntity);
    void            moveHitBox(Entity *entity);

private:
    Map*            _map;
};