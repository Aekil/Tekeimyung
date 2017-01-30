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
    void    ScriptSystem::initialize(EntityManager &em);
END_SYSTEM(ScriptSystem)
