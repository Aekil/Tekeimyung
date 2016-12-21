/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/Map.hpp>

#define SCRIPT_SYSTEM_NAME "Script system"

START_SYSTEM(ScriptSystem)
public:
    ScriptSystem();
    virtual     ~ScriptSystem();
    virtual void update(EntityManager &em, float elapsedTime);
    
    ADD_MONITORING_VAR
END_SYSTEM(ScriptSystem)
