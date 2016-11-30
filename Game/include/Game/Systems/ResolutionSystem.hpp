#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define RESOLUTION_SYSTEM_NAME "Resolution system"

START_SYSTEM(ResolutionSystem)
public:
    ResolutionSystem();
    virtual                                     ~ResolutionSystem();
    virtual void                                update(EntityManager &em, float elapsedTime);

private:
    ADD_MONITORING_VAR
END_SYSTEM(ResolutionSystem)
