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

        for (std::map<int, eCollisionState>::iterator it = resolutionComponent->resolutions.begin(); it != resolutionComponent->resolutions.end(); ++it)
        {
            if (it->second == eCollisionState::ENTERING_COLLISION)
            {
                if (scriptComponent != nullptr)
                {
                    for (auto&& script : scriptComponent->scriptInstances)
                    {
                        script->OnCollisionEnter(em.getEntity(it->first));
                    }
                }
                resolutionComponent->resolutions[it->first] = eCollisionState::IS_COLLIDING;
            }
            else if (it->second == eCollisionState::EXIT_COLLISION)
            {
                if (scriptComponent != nullptr)
                {
                    for (auto&& script : scriptComponent->scriptInstances)
                    {
                        script->OnCollisionExit(em.getEntity(it->first));
                    }
                }
                resolutionComponent->resolutions[it->first] = eCollisionState::NO_COLLISION;
            }
        }
        
    });
}
