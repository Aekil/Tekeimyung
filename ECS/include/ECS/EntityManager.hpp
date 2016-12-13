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
public:
    EntityManager(World& world);
    ~EntityManager();

    Entity*                                         createEntity();

    void                                            destroyEntity(Entity* entity);
    void                                            destroyEntityRegister(Entity* entity);
    void                                            destroyEntities();

    std::unordered_map<uint32_t, Entity*>&          getEntities();
    Entity*                                         getEntity(uint32_t id) const;

    void                                            notifyEntityNewComponent(Entity* entity, sComponent* component);
    void                                            notifyEntityRemovedComponent(Entity* entity, sComponent* component);

private:
    static int                                      _entityId;
    std::unordered_map<uint32_t, Entity*>           _entities;
    std::vector<Entity*>                            _entitiesToDestroy;

    World&                                          _world;
};
