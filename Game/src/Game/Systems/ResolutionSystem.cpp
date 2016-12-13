/**
* @Author   Simon Ambroise
*/

#include <Game/Components.hh>

#include <Game/Systems/ResolutionSystem.hpp>
#include <Game/Utils/Resolutions.hpp>

ResolutionSystem::ResolutionSystem()
{
    addDependency<sResolutionComponent>();
    _keyMonitoring = MonitoringDebugWindow::getInstance()->registerSystem(RESOLUTION_SYSTEM_NAME);
}

ResolutionSystem::~ResolutionSystem() { }

void ResolutionSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;

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
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_keyMonitoring, timer.getElapsedTime(), nbEntities);
}
