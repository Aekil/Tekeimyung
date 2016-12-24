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

        scriptComponent->scriptInstance = ScriptFactory::create(scriptComponent->scriptName);
        scriptComponent->scriptInstance->SetEntity(entity);

        scriptComponent->scriptInstance->Start();
    });
}

void    ScriptSystem::update(EntityManager &em, float elapsedTime)
{
    static bool isInitialized = false;
    Timer timer;
    uint32_t nbEntities = 0;
       
    if (!isInitialized)
    {
        this->initialize(em);
        isInitialized = true;
    }
    else 
    {
        forEachEntity(em, [&](Entity *entity)
        {
            auto entityClass = entity->getComponent<sScriptComponent>()->scriptInstance;

            entityClass->Update(elapsedTime);
            ++nbEntities;
        });
    }

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}