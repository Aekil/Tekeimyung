/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <ECS/System.hpp>

System::System(uint32_t id): _id(id) {}

System::~System() {}

bool    System::init()
{
    return (true);
}

void    System::forEachEntity(EntityManager& em, std::function<void(Entity* entity)> callback)
{
    for (uint32_t idx = 0; idx < this->_entities.size(); idx++)
    {
        Entity* entity = em.getEntity(this->_entities[idx]);

        if (this->hasDependencyDisabled(entity))
            continue;

        if (entity)
            callback(entity);
    }
}

uint32_t    System::getId() const
{
    return (_id);
}

uint32_t    System::getEntitiesNb() const
{
    return ((uint32_t)_entities.size());
}

bool    System::hasDependencyDisabled(Entity* entity) const
{
    for (auto componentHash : _components)
    {
        if (!entity->getComponent(componentHash)->enabled)
            return (true);
    }

    return (false);
}

bool    System::hasDependency(sComponent* component) const
{
    for (auto componentHash : _components)
    {
        if (componentHash == component->id)
            return (true);
    }
    return (false);
}

bool    System::entityMatchDependencies(Entity* entity) const
{
    for (auto component: _components)
    {
        if (!entity->hasComponent(component))
            return (false);
    }
    return (true);
}

bool    System::onEntityNewComponent(Entity* entity, sComponent* component)
{
    // The system is dependant of the component
    if (hasDependency(component) &&
        // The entity has got all components to be in the system
        entityMatchDependencies(entity) &&
        // The entity is not already in the system list
        std::find(_entities.cbegin(), _entities.cend(), entity->id) == _entities.cend())
    {
        _entities.push_back(entity->id);
        return (true);
    }
    return (false);
}

bool    System::onEntityRemovedComponent(Entity* entity, sComponent* component)
{
    // The system is dependant of the component
    if (hasDependency(component))
    {
        auto foundEntity = std::find(_entities.cbegin(), _entities.cend(), entity->id);
        // The entity is in the system list
        if (foundEntity != _entities.cend())
        {
            _entities.erase(foundEntity);
            return (true);
        }
    }
    return (false);
}

bool    System::onEntityDeleted(Entity* entity)
{
    auto foundEntity = std::find(_entities.cbegin(), _entities.cend(), entity->id);
    // The entity is in the system list
    if (foundEntity != _entities.cend())
    {
        _entities.erase(foundEntity);
        return (true);
    }
    return (false);
}
