#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define AI_SYSTEM_NAME "AI system"

START_SYSTEM(AISystem)
public:
    AISystem();
    virtual ~AISystem();
    virtual void update(EntityManager &em, float elapsedTime);

private:
    ADD_MONITORING_VAR
END_SYSTEM(AISystem)
