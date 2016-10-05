#include <algorithm>
#include "EntityManager.hpp"
#include <algorithm>
#include <random>

int                         EntityManager::_entityId = 0;


EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

Entity* EntityManager::createEntity()
{
    Entity* entity = new Entity(_entityId++);

    _entities[entity->id] = entity;
    return (entity);
}

void    EntityManager::destroyEntity(Entity* entity)
{
    std::for_each(entity->_components.begin(), entity->_components.end(), [](sComponent* component)
    {
        delete component;
    });
    entity->_components.clear();

    _entities.erase(entity->id);
}

std::unordered_map<uint32_t, Entity*>& EntityManager::getEntities()
{
    return (_entities);
}

Entity* EntityManager::getEntity(uint32_t id) const
{
    return _entities.at(id);
}
