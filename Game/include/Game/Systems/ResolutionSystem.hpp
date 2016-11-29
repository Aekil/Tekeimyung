#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp> 

#define RESOLUTION_SYSTEM_NAME "Resolution system"

class ResolutionSystem : public System
{
public:
    ResolutionSystem();
    virtual                                     ~ResolutionSystem();
    virtual void                                update(EntityManager &em, float elapsedTime);

private:
    uint16_t                                    _keyMonitoring;
    tMonitoring                                 _data;
};
