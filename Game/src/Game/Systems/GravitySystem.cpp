#include <Game/Components.hh>

#include <Game/Systems/GravitySystem.hpp>

GravitySystem::GravitySystem()
{
    this->addDependency<sGravityComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();

    _keyMonitoring = MonitoringDebugWindow::getInstance()->registerSystem(GRAVITY_SYSTEM_NAME);
}

void GravitySystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;

    this->forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sGravityComponent* gravity = entity->getComponent<sGravityComponent>();

        direction->value += gravity->value * elapsedTime;
    });

    _data.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_keyMonitoring, _data);
}
