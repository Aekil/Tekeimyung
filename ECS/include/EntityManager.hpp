#pragma once

#include <unordered_map>
#include <cstdint>
#include "Entity.hpp"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    Entity*                                         createEntity();
    void                                            destroyEntity(Entity* entity);
    std::unordered_map<uint32_t, Entity*>&          getEntities();
    Entity*                                         getEntity(uint32_t id) const;

private:
    static int                                      _entityId;
    std::unordered_map<uint32_t, Entity*>           _entities;
};
