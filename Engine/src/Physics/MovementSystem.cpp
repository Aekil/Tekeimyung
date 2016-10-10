#include "Core/Components.hh"

#include <cmath>
#include "Physics/MovementSystem.hpp"

MovementSystem::MovementSystem(Map* map): _map(map)
{
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void    MovementSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent *position = entity->getComponent<sPositionComponent>();

        position->x += direction->x * elapsedTime;
        position->y += direction->y * elapsedTime;
    });
}
