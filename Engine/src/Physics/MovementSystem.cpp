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
        glm::ivec3 oldPosition(floor(position->value.x), floor(position->value.y), position->z);

        position->value += direction->value * elapsedTime;

        glm::ivec3 newPosition(floor(position->value.x), floor(position->value.y), position->z);
        if (_map->isValidPosition(newPosition) && (newPosition.x != oldPosition.x || newPosition.y != oldPosition.y))
        {
            std::cout << "X: " << newPosition.x << " Y: " << newPosition.y << "  Z: " << oldPosition.z << std::endl;
            (*_map)[oldPosition.z][oldPosition.y][oldPosition.x].set(Map::eObjType::DYNAMIC, 0);
            (*_map)[newPosition.z][newPosition.y][newPosition.x].set(Map::eObjType::DYNAMIC, entity->id);
        }
    });
}
