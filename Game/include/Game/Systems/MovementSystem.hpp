/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/Map.hpp>

#define MOVEMENT_SYSTEM_NAME "Movement system"

START_SYSTEM(MovementSystem)
public:
    MovementSystem(Map* map);
    virtual ~MovementSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    Map*            _map;

    ADD_MONITORING_VAR
END_SYSTEM(MovementSystem)
