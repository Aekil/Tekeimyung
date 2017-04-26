/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Debug/MonitoringDebugWindow.hpp>

START_SYSTEM(ScriptSystem)
public:
    ScriptSystem();
    virtual     ~ScriptSystem();
    virtual void update(EntityManager &em, float elapsedTime);
END_SYSTEM(ScriptSystem)
