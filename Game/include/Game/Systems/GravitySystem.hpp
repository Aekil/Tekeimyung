#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define GRAVITY_SYSTEM_NAME "Gravity system"

START_SYSTEM(GravitySystem)
public:
    GravitySystem();
    virtual ~GravitySystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    uint16_t        _keyMonitoring;
    tMonitoring     _data;
END_SYSTEM(GravitySystem)
