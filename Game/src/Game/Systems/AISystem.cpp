#include <Engine/Window/GameWindow.hpp>

#include <Game/Components.hh>

#include <Game/Systems/AISystem.hpp>

AISystem::AISystem()
{
    addDependency<sAIComponent>();
    addDependency<sDirectionComponent>();

    _keyMonitoring = MonitoringDebugWindow::getInstance()->registerSystem(AI_SYSTEM_NAME);
}

AISystem::~AISystem() {}

void    AISystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;

    forEachEntity(em, [&](Entity *entity) {
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

        direction->value.x = direction->speed;
        direction->orientation = eOrientation::SE;
        direction->moved = true;
    });

    _data.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_keyMonitoring, _data);
}
