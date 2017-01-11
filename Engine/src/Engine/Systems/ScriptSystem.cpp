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

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(SCRIPT_SYSTEM_NAME);
}

ScriptSystem::~ScriptSystem() {}

void    ScriptSystem::initialize(EntityManager &em)
{
    this->forEachEntity(em, [&](Entity* entity)
    {
        auto scriptComponent = entity->getComponent<sScriptComponent>();

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
    });
}

void    ScriptSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;

    this->initialize(em);
    forEachEntity(em, [&](Entity *entity)
    {
        for (auto&& script : entity->getComponent<sScriptComponent>()->scriptInstances)
            script->Update(elapsedTime);

        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}