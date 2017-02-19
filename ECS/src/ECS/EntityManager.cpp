/**
* @Author   Guillaume Labey
*/

#include <algorithm>
#include <random>

#include <ECS/World.hpp>
#include <ECS/EntityManager.hpp>
#include <ECS/EntityPool.hpp>

EntityManager::EntityManager(World& world): _world(world)
{
    _entityPool = std::make_unique<EntityPool>(this, 100);
}

EntityManager::~EntityManager() {}

Entity* EntityManager::createEntity()
{
    Entity* entity = _entityPool->allocate();

    _entities.push_back(entity);
    return (entity);
}

void    EntityManager::destroyEntity(Entity* entity)
{
    _world.notifyEntityDeleted(entity);
    std::for_each(entity->_components.begin(), entity->_components.end(), [](sComponent* component)
    {
        delete component;
    });
    entity->_components.clear();

    removeEntityFromTagGroup(entity, entity->getTag());
    _entities.erase(std::find(_entities.cbegin(), _entities.cend(), entity));

    _entityPool->free(entity);
}

void    EntityManager::destroyEntityRegister(Entity* entity)
{
    if (entity)
    {
        _entitiesToDestroy.push_back(entity);
    }
}

void    EntityManager::destroyEntities()
{
    if (_entitiesToDestroy.empty())
        return;

    uint32_t i = 0;
    uint32_t len = (uint32_t)_entitiesToDestroy.size();
    for (i; i < len; ++i)
    {
        destroyEntity(_entitiesToDestroy[i]);
    }

    // Other entities have been added to _entitiesToDestroy during destroy
    if (len != _entitiesToDestroy.size())
    {
        _entitiesToDestroy.erase(_entitiesToDestroy.begin(), _entitiesToDestroy.begin() + len);
    }
    else
    {
        _entitiesToDestroy.clear();
    }
}

void    EntityManager::destroyAllEntities()
{
    auto it = _entities.begin();
    for (it; it != _entities.end();)
    {
        Entity* entity = *it;
        it = _entities.erase(it);
        destroyEntity(entity);
    }
    _entitiesToDestroy.clear();
}

std::vector<Entity*>& EntityManager::getEntities()
{
    return (_entities);
}

const std::vector<Entity*>& EntityManager::getEntitiesByTag(const std::string& tag)
{
    return (_entitiesTagGroups[tag]);
}

Entity* EntityManager::getEntity(uint32_t id) const
{
    return (_entityPool->getEntity(id));
}

void    EntityManager::notifyEntityNewComponent(Entity* entity, sComponent* component)
{
    _world.notifyEntityNewComponent(entity, component);
}

void    EntityManager::notifyEntityRemovedComponent(Entity* entity, sComponent* component)
{
    _world.notifyEntityRemovedComponent(entity, component);
}

void    EntityManager::notifyEntityCreated(Entity* entity)
{
    _world.notifyEntityCreated(entity);
}

void    EntityManager::addEntityToTagGroup(Entity* entity, const std::string& name)
{
    if (name.size() == 0)
        return;

    _entitiesTagGroups[name].push_back(entity);
}

void    EntityManager::removeEntityFromTagGroup(Entity* entity, const std::string& name)
{
    if (name.size() == 0)
        return;

    auto& tagGroup = _entitiesTagGroups.find(name);
    if (tagGroup == _entitiesTagGroups.end())
        return;

    auto& entityFind = std::find(tagGroup->second.cbegin(), tagGroup->second.cend(), entity);
    if (entityFind != tagGroup->second.cend())
    {
        tagGroup->second.erase(entityFind);
    }
}
