/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define SCRIPT_SYSTEM_NAME "Script system"

START_SYSTEM(ScriptSystem)
public:
    ScriptSystem();
    virtual     ~ScriptSystem();
    virtual void update(EntityManager &em, float elapsedTime);

private:
    void    ScriptSystem::firstStart(EntityManager &em);
    
    ADD_MONITORING_VAR
END_SYSTEM(ScriptSystem)
