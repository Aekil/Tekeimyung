#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Game/Components.hh>

#include <Game/Systems/BuildSystem.hpp>

BuildSystem::BuildSystem(Map* map): _map(map)
{
    addDependency<sPlayerComponent>();
    addDependency<sPositionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(BUILD_SYSTEM_NAME);
}

BuildSystem::~BuildSystem() {}

bool    BuildSystem::init()
{
    return (true);
}

void    BuildSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;

    forEachEntity(em, [&](Entity* entity) {
        sPlayerComponent* player = entity->getComponent<sPlayerComponent>();
        sPositionComponent* pos = entity->getComponent<sPositionComponent>();

        drawRange(em, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);
        player->lastPos = glm::ivec3(pos->value, pos->z);
        drawRange(em, player->rangeColor, glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), (uint32_t)_entities.size());
}

void    BuildSystem::drawRange(EntityManager &em, const glm::vec4& color, const glm::ivec3& playerPos, uint32_t range)
{
    glm::ivec3 rangePos = playerPos - (int)range;
    for (uint32_t x = 0; x <= range * 2; ++x)
    {
        for (uint32_t y = 0; y <= range * 2; ++y)
        {
            glm::ivec3 pos = glm::ivec3(rangePos.x + x, rangePos.y + y, playerPos.z - 1);
            if (!_map->isValidPosition(pos))
                continue;

            uint32_t entityId = (*_map)[pos.z][pos.y][pos.x].get();
            Entity* entity = em.getEntity(entityId);

            if (entity)
            {
                sRenderComponent* render = entity->getComponent<sRenderComponent>();
                if (render)
                    render->color = color;
            }
        }
    }
}
