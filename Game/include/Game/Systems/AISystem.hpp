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
    uint16_t        _keyMonitoring;
    tMonitoring     _data;
END_SYSTEM(AISystem)
