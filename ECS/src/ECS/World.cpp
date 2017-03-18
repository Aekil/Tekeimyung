/**
* @Author   Guillaume Labey
*/

#include <ECS/World.hpp>

World::World()
{
    _entityManager = std::make_unique<EntityManager>(*this);
}

World::~World() {}

EntityManager*	World::getEntityManager()
{
    return (_entityManager.get());
}

std::vector<std::unique_ptr<System> >&	World::getSystems()
{
    return (_systems);
}

void    World::notifyEntityNewComponent(Entity* entity, sComponent* component)
{
    for (auto& system_: _systems)
    {
        system_->onEntityNewComponent(entity, component);
    }
}

void    World::notifyEntityRemovedComponent(Entity* entity, sComponent* component)
{
    for (auto& system_: _systems)
    {
        system_->onEntityRemovedComponent(entity, component);
    }
}

void    World::notifyEntityCreated(Entity* entity)
{
    for (auto& system_: _systems)
    {
        system_->onEntityCreated(entity);
    }
}

void    World::notifyEntityDeleted(Entity* entity)
{
    for (auto& system_: _systems)
    {
        system_->onEntityDeleted(entity);
    }
}
