#include <cmath>
#include <algorithm>

#include "Core/Components.hh"

#include "Systems/MovementSystem.hpp"

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

        position->value += direction->value * elapsedTime;

        position->value.x = std::max(0.0f, position->value.x);
        position->value.x = std::min((float)_map->getWidth() - 0.01f, position->value.x);
        position->value.y = std::max(0.0f, position->value.y);
        position->value.y = std::min((float)_map->getHeight() - 0.01f, position->value.y);
    });
}
