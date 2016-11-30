#include <Game/Components.hh>
#include <Game/Utils/Resolutions.hpp>

#include <Game/Systems/ResolutionSystem.hpp>

ResolutionSystem::ResolutionSystem()
{
    addDependency<sResolutionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(RESOLUTION_SYSTEM_NAME);
}

ResolutionSystem::~ResolutionSystem() { }

void ResolutionSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;

    this->forEachEntity(em, [&](Entity* entity)
    {
        sResolutionComponent* resolutionComponent = entity->getComponent<sResolutionComponent>();

        if (resolutionComponent->collidingState == eCollisionState::ENTERING_COLLISION)
        {
            resolutionComponent->onCollisionEnter(resolutionComponent->entityId);
            resolutionComponent->collidingState = eCollisionState::IS_COLLIDING;
        }
        else if (resolutionComponent->collidingState == eCollisionState::EXIT_COLLISION)
        {
            resolutionComponent->onCollisionExit(resolutionComponent->entityId);
            resolutionComponent->collidingState = eCollisionState::NO_COLLISION;
        }
    });

    _monitoringData.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, _monitoringData);
}