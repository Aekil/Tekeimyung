/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <ECS/Component.hh>
#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

START_COMPONENT(sScriptComponent)
virtual sComponent*     clone()
{
    sScriptComponent*    component = new sScriptComponent();
    component->update(this);

    return (component);
}

virtual void            update(sScriptComponent* component)
{
    for (auto& script: component->scripts)
    {
        JsonValue json = script->saveToJson();

        auto scriptInstance = ScriptFactory::create(script->getName());

        // Little trick to update the component ._.
        scriptInstance->loadFromJson(json);

        this->scripts.push_back(std::move(scriptInstance));
    }
}

virtual void            update(sComponent* component)
{
    update(static_cast<sScriptComponent*>(component));
}

bool                    hasScript(const char* name)
{
    for (const auto& script: scripts)
    {
        if (script->getName() == name)
        {
            return (true);
        }
    }
    return (false);
}

BaseScript*             getScript(const char* name)
{
    uint32_t i = 0;
    for (const auto& script: scripts)
    {
        if (script->getName() == name)
        {
            if (!script->getEntity())
                script->setEntity(entity);

            // If the user need the script before it's initialized, auto initialized it
            if (!script->isInitialized)
            {
                script->start();
                script->isInitialized = true;
            }
            return (script.get());
        }
        i++;
    }

    return nullptr;
}

template <typename T>
T*             getScript(const char* name)
{
    BaseScript* script = getScript(name);
    if (script)
        return static_cast<T*>(script);
    return nullptr;
}

template<typename T>
static T*              getEntityScript(Entity* entity, const char* scriptName)
{
    sScriptComponent* scriptComp = entity->getComponent<sScriptComponent>();
    if (!scriptComp)
    {
        return (nullptr);
    }

    return scriptComp->getScript<T>(scriptName);
}

std::vector<std::unique_ptr<BaseScript> > scripts;

BaseScript* selectedScript{nullptr}; // Only used for editor
END_COMPONENT(sScriptComponent)
