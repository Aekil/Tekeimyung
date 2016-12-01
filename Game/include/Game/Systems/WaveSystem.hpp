#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

#define WAVE_SYSTEM_NAME "Wave system"

START_SYSTEM(WaveSystem)
public:
    WaveSystem(Map* map);
    virtual ~WaveSystem();

    virtual void            update(EntityManager &em, float elapsedTime);

    static uint32_t         createWave(Map* map, const glm::vec3& pos, eArchetype type);
    static void             setNbEntities(EntityManager &em, uint32_t waveEntityID, int entityNb);
    static void             setSecBeforeFirstSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeFirstSpawn);
    static void             setSecBeforeEachSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeEachSpawn);
    static void             setAllFields(EntityManager &em, uint32_t waveEntityID, sWaveComponent &waveData);
        
private:
    Entity*                 createEntityFromWave(Map* map, const glm::vec3& pos, eArchetype type);

private:
    Map*                    _map;

    ADD_MONITORING_VAR
END_SYSTEM(WaveSystem)
