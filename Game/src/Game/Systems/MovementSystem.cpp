/**
* @Author   Guillaume Labey
*/

#include <cmath>
#include <algorithm>

#include <Game/Components.hh>

#include <Game/Systems/MovementSystem.hpp>

MovementSystem::MovementSystem(Map* map): _map(map)
{
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
    this->addDependency<sTransformComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(MOVEMENT_SYSTEM_NAME);
}

void    MovementSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;

    forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();
        sPositionComponent *position = entity->getComponent<sPositionComponent>();
        sTransformComponent *transform = entity->getComponent<sTransformComponent>();

        // Is moving
        if (!(direction->value.x == 0.0f && direction->value.y == 0.0f))
        {
            glm::vec3 prevPos = Map::mapToGraphPosition(position->value, position->z);
            position->value += direction->value * elapsedTime;

            // Limit position to don't go out of the map
            position->value.x = std::max(0.0f, position->value.x);
            position->value.x = std::min((float)_map->getWidth() - 0.01f, position->value.x);
            position->value.y = std::max(0.0f, position->value.y);
            position->value.y = std::min((float)_map->getHeight() - 0.01f, position->value.y);

            transform->pos += Map::mapToGraphPosition(position->value, position->z) - prevPos;
            transform->needUpdate = true;
        }
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}
