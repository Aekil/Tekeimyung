#pragma once

#include <list>
#include "Entity.hpp"

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    Entity*                     createEntity();
    void                        destroyEntity(Entity* entity);
    std::list<Entity*>&         getEntities();

    template <class Compare>
    void                        sortEntities(Compare comp)
    {
        _entities.sort(comp);
    }

private:
    static int                  _entityId;
    std::list<Entity*>          _entities;
};