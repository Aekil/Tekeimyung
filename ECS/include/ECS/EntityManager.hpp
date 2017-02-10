/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <cstdint>

#include <ECS/Entity.hpp>

class World;

class EntityManager
{
friend Entity;

public:
    EntityManager(World& world);
    ~EntityManager();

    Entity*                                         createEntity();

    void                                            destroyEntity(Entity* entity);
    void                                            destroyEntityRegister(Entity* entity);
    void                                            destroyEntities();
    void                                            destroyAllEntities();

    std::unordered_map<uint32_t, Entity*>&          getEntities();
    const std::vector<Entity*>&                     getEntitiesByTag(const std::string& tag);
    Entity*                                         getEntity(uint32_t id) const;

    // This function is not notified by the entity or the entity manager
    // It has to be called when an entity is created (all the components created too)
    void                                            notifyEntityCreated(Entity* entity);

private:
    void                                            notifyEntityNewComponent(Entity* entity, sComponent* component);
    void                                            notifyEntityRemovedComponent(Entity* entity, sComponent* component);
    void                                            addEntityToTagGroup(Entity* entity, const std::string& name);
    void                                            removeEntityFromTagGroup(Entity* entity, const std::string& name);

private:
    static int                                      _entityId;

    // Store entities by id
    std::unordered_map<uint32_t, Entity*>           _entities;

    // Store entities by tag
    std::unordered_map<std::string, std::vector<Entity*> >        _entitiesTagGroups;
    std::vector<Entity*>                            _entitiesToDestroy;

    World&                                          _world;
};
