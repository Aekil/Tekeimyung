/**
* @Author   Simon Ambroise
*/

#include <Engine/Components.hh>

#include <Engine/Utils/Logger.hpp>

#include <Engine/Systems/ResolutionSystem.hpp>

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
        sScriptComponent* scriptComponent = entity->getComponent<sScriptComponent>();

        if (resolutionComponent->collidingState == eCollisionState::ENTERING_COLLISION)
        {
            if (scriptComponent != nullptr)
            {
                for (auto&& script : scriptComponent->scriptInstances)
                {
                    script->OnCollisionEnter(em.getEntity(resolutionComponent->entityId));
                }
            }
            resolutionComponent->collidingState = eCollisionState::IS_COLLIDING;
        }
        else if (resolutionComponent->collidingState == eCollisionState::EXIT_COLLISION)
        {
            if (scriptComponent != nullptr)
            {
                for (auto&& script : scriptComponent->scriptInstances) 
                {
                    script->OnCollisionExit(em.getEntity(resolutionComponent->entityId));
                }
            }
            resolutionComponent->collidingState = eCollisionState::NO_COLLISION;
        }
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_keyMonitoring, timer.getElapsedTime(), nbEntities);
}
