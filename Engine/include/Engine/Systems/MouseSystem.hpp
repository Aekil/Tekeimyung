/**
* @Author   Simon Ambroise
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#define MOUSE_SYSTEM_NAME "Mouse system"

START_SYSTEM(MouseSystem)
public:
    MouseSystem();
    virtual ~MouseSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    void hoverEntity(EntityManager&);

    Entity* previousEntity = nullptr;

    ADD_MONITORING_VAR
END_SYSTEM(MouseSystem)
