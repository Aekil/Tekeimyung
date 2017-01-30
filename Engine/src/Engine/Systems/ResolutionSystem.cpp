/**
* @Author   Simon Ambroise
*/

#include <Engine/Components.hh>

#include <Engine/Utils/Logger.hpp>

#include <Engine/Systems/ResolutionSystem.hpp>

ResolutionSystem::ResolutionSystem()
{
    addDependency<sResolutionComponent>();
}

ResolutionSystem::~ResolutionSystem() { }

void ResolutionSystem::update(EntityManager &em, float elapsedTime)
{
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
    });
}
