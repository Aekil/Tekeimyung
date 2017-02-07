/**
* @Author   Simon AMBROISE
*/

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Engine/Systems/ScriptSystem.hpp>

#include <Engine/Core/ScriptFactory.hpp>

ScriptSystem::ScriptSystem()
{
    this->addDependency<sScriptComponent>();
}

ScriptSystem::~ScriptSystem() {}

void    ScriptSystem::initializeScript(Entity* entity, sScriptComponent* scriptComponent)
{
    if (!scriptComponent->isInitialized)
    {
        for (auto&& scriptName : scriptComponent->scriptNames)
        {
            auto scriptInstance = ScriptFactory::create(scriptName);
            scriptInstance->SetEntity(entity);

            scriptInstance->Start();
            scriptComponent->scriptInstances.push_back(scriptInstance);
        }

        scriptComponent->isInitialized = true;
    }
}

void    ScriptSystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity)
    {
        sScriptComponent* scriptComponent = entity->getComponent<sScriptComponent>();

        this->initializeScript(entity, scriptComponent);
        for (auto&& script : scriptComponent->scriptInstances)
            script->Update(elapsedTime);
    });
}
