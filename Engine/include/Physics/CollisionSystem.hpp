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

private:
    Map*            _map;
};
