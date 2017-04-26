/**
* @Author   Simon AMBROISE
*/

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Debug/Debug.hpp>

#include <Engine/Systems/ScriptSystem.hpp>

#include <Engine/Core/ScriptFactory.hpp>

ScriptSystem::ScriptSystem()
{
    this->addDependency<sScriptComponent>();
}

ScriptSystem::~ScriptSystem() {}

void    ScriptSystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity)
    {
        sScriptComponent* scriptComponent = entity->getComponent<sScriptComponent>();

        for (auto&& script : scriptComponent->scripts)
        {
            if (!script->getEntity())
                script->setEntity(entity);

            // We can't initialize the scripts in ScriptSystem::initializeScript
            // because the entity components used in BaseScript::Start may not be added
            if (!script->isInitialized)
            {
                script->start();
                script->isInitialized = true;
            }
            script->update(elapsedTime);
        }
    });
}
