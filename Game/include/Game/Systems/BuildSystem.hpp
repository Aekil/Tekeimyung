#pragma once

#include <ECS/System.hpp>

#include <Game/Map.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define BUILD_SYSTEM_NAME "Build system"

START_SYSTEM(BuildSystem)
public:
    BuildSystem(Map* map);
    virtual ~BuildSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

private:
    ADD_MONITORING_VAR

    Map*                                _map;
END_SYSTEM(BuildSystem)
