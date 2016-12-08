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

        player->lastPos = glm::vec3(pos->value, pos->z);
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), (uint32_t)_entities.size());
}
