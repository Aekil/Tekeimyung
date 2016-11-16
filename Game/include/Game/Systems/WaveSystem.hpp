#pragma once

#include <ECS/System.hpp>

#include <Engine/Utils/Timer.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

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
    //std::vector<Entity*>    _enemies;
};