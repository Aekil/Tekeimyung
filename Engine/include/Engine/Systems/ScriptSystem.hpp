/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

START_SYSTEM(ScriptSystem)
public:
    ScriptSystem();
    virtual     ~ScriptSystem();
    virtual void update(EntityManager &em, float elapsedTime);

private:
    void    ScriptSystem::initializeScript(Entity* entity, sScriptComponent* scriptComponent);
END_SYSTEM(ScriptSystem)
