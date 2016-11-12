#pragma once

#include <ECS/System.hpp>

#include <Game/Map.hpp>

class CollisionSystem : public System
{
public:
    CollisionSystem(Map* map);
    virtual ~CollisionSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    bool            isColliding(Entity *firstEntity, Entity *secondEntity);
    void            moveHitbox(Entity *entity);

private:
    Map*            _map;
};
