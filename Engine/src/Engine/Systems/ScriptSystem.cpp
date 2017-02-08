/**
* @Author   Simon AMBROISE
*/

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Engine/Systems/ScriptSystem.hpp>

#include <Engine/Core/ScriptFactory.hpp>

ScriptSystem::ScriptSystem()
{
    this->addDependency<sScriptComponent>();
}

ScriptSystem::~ScriptSystem() {}

void    ScriptSystem::initializeScript(Entity* entity, sScriptComponent* scriptComponent)
{
    for (auto&& scriptName : scriptComponent->scriptNames)
    {
        auto scriptInstance = ScriptFactory::create(scriptName);
        scriptInstance->SetEntity(entity);

        scriptComponent->scriptInstances.push_back(scriptInstance);
    }
}

void    ScriptSystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity)
    {
        sScriptComponent* scriptComponent = entity->getComponent<sScriptComponent>();

        for (auto&& script : scriptComponent->scriptInstances)
        {
            // We can't initialize the scripts in ScriptSystem::initializeScript
            // because the entity components used in BaseScript::Start may not be added
            if (!script->isInitialized)
            {
                script->Start();
                script->isInitialized = true;
            }
            script->Update(elapsedTime);
        }
    });
}

bool    ScriptSystem::onEntityNewComponent(Entity* entity, sComponent* component)
{
    // A new entity is added to the system
    if (System::onEntityNewComponent(entity, component))
    {
        ASSERT(component->id == sScriptComponent::identifier, "The new component should be a sScriptComponent");
        sScriptComponent* scriptComponent = static_cast<sScriptComponent*>(component);
        this->initializeScript(entity, scriptComponent);
        return (true);
    }
    return (false);
}
