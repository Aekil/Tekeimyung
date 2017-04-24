/**
* @Author   Guillaume Labey
*/

#include <algorithm>
#include <random>
#include <iostream>

#include <ECS/World.hpp>
#include <ECS/EntityManager.hpp>
#include <ECS/EntityPool.hpp>

EntityManager::EntityManager(World& world): _world(world)
{
    _entityPool = std::make_unique<EntityPool>(this, 100);
}

EntityManager::~EntityManager() {}

Entity* EntityManager::createEntity(bool store)
{
    Entity* entity = _entityPool->allocate();

    if (store)
    {
        _entities.push_back(entity);
    }
    return (entity);
}

void    EntityManager::destroyEntity(Entity* entity)
{
    _world.notifyEntityDeleted(entity);
    std::for_each(entity->_components.begin(), entity->_components.end(), [this, &entity](sComponent* component)
    {
        removeEntityFromComponentGroup(entity, component->id);
        delete component;
    });
    entity->_components.clear();

    removeEntityFromTagGroup(entity, entity->getTag());

    auto entityIt = std::find(_entities.cbegin(), _entities.cend(), entity);
    if (entityIt == _entities.cend())
    {
        std::cerr << "Warning: Attempt to delete entity " << entity->id << " which is already deleted" << std::endl;
        return;
    }
    _entities.erase(entityIt);
    entity->_free = true;
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
    // _entitiesToFree is used to free entities from pool
    // the next frame of the destroy
    for (Entity* entity: _entitiesToFree)
    {
        _entityPool->free(entity);
    }

    _entitiesToFree.clear();

    if (_entitiesToDestroy.empty())
        return;

    uint32_t i = 0;
    uint32_t len = (uint32_t)_entitiesToDestroy.size();
    _entitiesToFree.resize(len);
    for (i; i < len; ++i)
    {
        _entitiesToFree[i] = _entitiesToDestroy[i];
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
    for (std::size_t entitiesNb = _entities.size(); entitiesNb > 0; --entitiesNb)
    {
        Entity* entity = _entities[0];
        destroyEntity(entity);
    }
    _entitiesToDestroy.clear();
    _entitiesToFree.clear();
}

std::vector<Entity*>& EntityManager::getEntities()
{
    return (_entities);
}

const std::vector<Entity*>& EntityManager::getEntitiesByTag(const std::string& tag)
{
    return (_entitiesTagGroups[tag]);
}

Entity* EntityManager::getEntityByTag(const std::string& tag)
{
    const auto& tagGroup = _entitiesTagGroups[tag];

    if (tagGroup.size() > 0)
    {
        return (tagGroup[0]);
    }

    return (nullptr);
}

Entity* EntityManager::getEntity(uint32_t id) const
{
    return (_entityPool->getEntity(id));
}

void    EntityManager::notifyEntityNewComponent(Entity* entity, sComponent* component)
{
    addEntityToComponentGroup(entity, component->id);
    _world.notifyEntityNewComponent(entity, component);
}

void    EntityManager::notifyEntityRemovedComponent(Entity* entity, sComponent* component)
{
    removeEntityFromComponentGroup(entity, component->id);
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

void    EntityManager::addEntityToComponentGroup(Entity* entity, uint32_t componentHash)
{
    _entitiesComponentGroups[componentHash].push_back(entity);
}

void    EntityManager::removeEntityFromComponentGroup(Entity* entity, uint32_t componentHash)
{
    auto& componentGroup = _entitiesComponentGroups.find(componentHash);
    if (componentGroup == _entitiesComponentGroups.end())
        return;

    auto& entityFind = std::find(componentGroup->second.cbegin(), componentGroup->second.cend(), entity);
    if (entityFind != componentGroup->second.cend())
    {
        componentGroup->second.erase(entityFind);
    }
}
