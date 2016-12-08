#include <algorithm>

#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/Logger.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Components.hh>

#include <Game/Systems/BuildSystem.hpp>

BuildSystem::BuildSystem(EntityManager& em, Map* map): _map(map), _em(em)
{
    addDependency<sPlayerComponent>();
    addDependency<sPositionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(BUILD_SYSTEM_NAME);
}

BuildSystem::~BuildSystem() {}

bool    BuildSystem::init()
{
    _tower = EntityFactory::createEntity(eArchetype::TOWER_FIRE);
    return (true);
}

void    BuildSystem::update(EntityManager &em, float elapsedTime)
{
    Entity* selectedEntity = _map->getSelectedEntity();
    Timer timer;

    // Draw player range
    forEachEntity(em, [&](Entity* entity) {
        sPlayerComponent* player = entity->getComponent<sPlayerComponent>();
        sPositionComponent* pos = entity->getComponent<sPositionComponent>();

        // Clear previous range
        drawRange(em, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);
        player->lastPos = glm::ivec3(pos->value, pos->z);
        // Draw new previous range
        drawRange(em, player->rangeColor, glm::ivec3(player->lastPos.x, player->lastPos.y, player->lastPos.z), player->range);

        if (selectedEntity)
        {
            sPositionComponent* pos = selectedEntity->getComponent<sPositionComponent>();
            if (isInRange(player->lastPos, glm::ivec3(pos->value.x, pos->value.y, pos->z), player->range))
            {
                buildOnTile(glm::ivec3(pos->value.x, pos->value.y, pos->z));
            }
        }
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

bool    BuildSystem::isInRange(const glm::ivec3& playerPos, const glm::ivec3& objPos, uint32_t range) const
{
    glm::ivec3 rangePos = playerPos - (int)range;

    return (objPos.z == playerPos.z - 1 &&
        objPos.x >= rangePos.x &&
        objPos.x <= rangePos.x * 2 &&
        objPos.y >= rangePos.y &&
        objPos.y <= rangePos.y * 2);
}

void    BuildSystem::buildOnTile(const glm::ivec3& pos)
{
    sTransformComponent* transform = _tower->getComponent<sTransformComponent>();
    transform->pos = Map::mapToGraphPosition(glm::vec2(pos.x, pos.y), (float)pos.z + 1);
    transform->needUpdate = true;
}
