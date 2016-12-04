#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

#define SPAWNER_SYSTEM_NAME "Spawner system"

START_SYSTEM(SpawnerSystem)
public:
    SpawnerSystem(Map* map);
    virtual ~SpawnerSystem();

    virtual void            update(EntityManager &em, float elapsedTime);

    static uint32_t         createSpawner(Map* map, const glm::vec3& pos, eArchetype type);
    static void             setNbEntities(EntityManager &em, uint32_t spawnerEntityID, int entityNb);
    static void             setSecBeforeFirstSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeFirstSpawn);
    static void             setSecBeforeEachSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeEachSpawn);
    static void             setSecBeforeLastSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeLastSpawn);
    static void             setAllFields(EntityManager &em, uint32_t spawnerEntityID, tSpawnerData &spawnerData);

    static tSpawnerData*    getStructData(int nbEnt, float firstTime, float eachTime, float lastTime);
        
private:
    Entity*                 createEntityFromSpawner(Map* map, const glm::vec3& pos, eArchetype type);
    void                    spawn(EntityManager &em, Entity* entity, sSpawnerComponent* spawnerComponent, bool *skipTimeFirstSpawn);

private:
    Map*                    _map;

    ADD_MONITORING_VAR
END_SYSTEM(SpawnerSystem)
