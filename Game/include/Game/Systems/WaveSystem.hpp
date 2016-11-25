#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

#define MONITORING_NAME "Wave system"

class   WaveSystem : public System
{
public:
    WaveSystem(Map* map);
    virtual ~WaveSystem();

    virtual void            update(EntityManager &em, float elapsedTime);
    
    static Entity*          createEntity(Map* map, const glm::vec3& pos, eArchetype type);

private:
    Timer*                  _timer;
    Map*                    _map;
    uint16_t                _keyMonitoring;
    //std::vector<Entity*>    _enemies;
};