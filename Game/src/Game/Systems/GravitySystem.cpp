#include <Game/Components.hh>

#include <Game/Systems/GravitySystem.hpp>

GravitySystem::GravitySystem()
{
    this->addDependency<sGravityComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(GRAVITY_SYSTEM_NAME);
}

void GravitySystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;

    this->forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sGravityComponent* gravity = entity->getComponent<sGravityComponent>();

        direction->value += gravity->value * elapsedTime;
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}
