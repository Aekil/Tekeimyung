/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/Map.hpp>

#define TOWER_AI_SYSTEM_NAME "TowerAI system"

START_SYSTEM(TowerAISystem)
public:
    TowerAISystem(Map* map);
    virtual     ~TowerAISystem();
    virtual void update(EntityManager &em, float elapsedTime);

private:
    uint32_t    getNearestEntityInRange(Entity* tower, std::list<uint32_t>& entities, EntityManager &em);
    bool        isEntityInRange(Entity* tower, Entity* potentialTarget);
    Entity*     createFireball(Entity* shooter, Entity* target);
    void        fire(Entity* shooter, Entity* enemy);

private:
    Map*            _map;
    
    ADD_MONITORING_VAR
END_SYSTEM(TowerAISystem)
