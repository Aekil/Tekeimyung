#pragma once

#include <vector>
#include "Entity.hpp"

class EntityManager
{
public:
    EntityManager() {}
    ~EntityManager() {}
    Entity*						createEntity();
    void 						destroyEntity(Entity* entity);
    std::vector<Entity*>&		getEntities();

private:
    static int                  _entityId;
    std::vector<Entity*>        _entities;
};