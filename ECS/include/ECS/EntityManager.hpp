/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <ECS/Entity.hpp>
#include <ECS/EntityPool.hpp>

class World;

class EntityManager
{
friend Entity;

public:
    EntityManager() = delete;
    EntityManager(World& world);
    ~EntityManager();

    Entity*                                         createEntity(bool store = true);

    void                                            destroyEntity(Entity* entity);
    void                                            destroyEntityRegister(Entity* entity);
    void                                            destroyEntities();
    void                                            destroyAllEntities();

    std::vector<Entity*>&                           getEntities();
    const std::vector<Entity*>&                     getEntitiesByTag(const std::string& tag);
    Entity*                                         getEntityByTag(const std::string& tag);

    template<typename T>
    const std::vector<Entity*>&                     getEntitiesByComponent()
    {
        return (_entitiesComponentGroups[T::identifier]);
    }

    Entity*                                         getEntity(uint32_t id) const;

    // This function is not notified by the entity or the entity manager
    // It has to be called when an entity is created (all the components created too)
    void                                            notifyEntityCreated(Entity* entity);

private:
    void                                            notifyEntityNewComponent(Entity* entity, sComponent* component);
    void                                            notifyEntityRemovedComponent(Entity* entity, sComponent* component);

    void                                            addEntityToTagGroup(Entity* entity, const std::string& name);
    void                                            removeEntityFromTagGroup(Entity* entity, const std::string& name);

    void                                            addEntityToComponentGroup(Entity* entity, uint32_t componentHash);
    void                                            removeEntityFromComponentGroup(Entity* entity, uint32_t componentHash);

private:
    // TODO: Replace Entity pointer with ID ?
    std::vector<Entity*>                            _entities;

    // Store entities by tag
    std::unordered_map<std::string, std::vector<Entity*> >        _entitiesTagGroups;

    // Store entities by component
    std::unordered_map<uint32_t, std::vector<Entity*> >        _entitiesComponentGroups;

    std::vector<Entity*>                            _entitiesToDestroy;
    std::vector<Entity*>                            _entitiesToFree;

    World&                                          _world;

    std::unique_ptr<EntityPool>                     _entityPool;
};
