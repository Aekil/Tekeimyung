#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define MONITORING_NAME "AI system"

class AISystem: public System
{
public:
    AISystem();
    virtual ~AISystem();
    virtual void update(EntityManager &em, float elapsedTime);

private:
    uint16_t        _keyMonitoring;
    tMonitoring     _data;
};
