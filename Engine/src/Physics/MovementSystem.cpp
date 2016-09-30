#include "Physics/MovementSystem.hpp"
#include "Core/Components.hh"

MovementSystem::MovementSystem()
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