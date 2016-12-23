/**
* @Author   Simon Ambroise
*/

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
    ADD_MONITORING_VAR
END_SYSTEM(GravitySystem)
