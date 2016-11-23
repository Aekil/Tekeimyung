#include <algorithm>
#include <random>

#include <ECS/EntityManager.hpp>

int                         EntityManager::_entityId = 1;


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

void    EntityManager::destroyEntityRegister(Entity* entity)
{
    _entitiesToDestroy.push_back(entity);
}

void    EntityManager::destroyEntities()
{
    if (_entitiesToDestroy.empty())
        return;

    for (auto entity : _entitiesToDestroy)
    {
        destroyEntity(entity);
    }
    _entitiesToDestroy.clear();
}

std::unordered_map<uint32_t, Entity*>& EntityManager::getEntities()
{
    return (_entities);
}

Entity* EntityManager::getEntity(uint32_t id) const
{
    auto &&it = _entities.find(id);

    return (it != _entities.end() ? it->second : nullptr);
}
