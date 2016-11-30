#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

#define WAVE_SYSTEM_NAME "Wave system"

START_SYSTEM(WaveSystem)
public:
    WaveSystem(Map* map);
    virtual ~WaveSystem();

    virtual void            update(EntityManager &em, float elapsedTime);

    static Entity*          createEntity(Map* map, const glm::vec3& pos, eArchetype type);

private:
    Timer*                  _timer;
    Map*                    _map;

    ADD_MONITORING_VAR
END_SYSTEM(WaveSystem)
