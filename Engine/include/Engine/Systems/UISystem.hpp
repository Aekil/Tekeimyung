/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>
#include <Engine/Components.hh>

#define UI_SYSTEM_NAME "UI system"

START_SYSTEM(UISystem)
public:
    UISystem();
    virtual ~UISystem();

    virtual void                        update(EntityManager &em, float elapsedTime);

    void                                onWindowResize(EntityManager &em);

private:
    void                                handleAlignment(EntityManager& em, Entity* entity, bool forceUpdate = false);

private:
    ADD_MONITORING_VAR
END_SYSTEM(UISystem)
