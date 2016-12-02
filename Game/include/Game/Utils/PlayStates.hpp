#pragma once

#include <Game/EntityFactory.hpp>
#include <Game/Map.hpp>

class PlayStates
{
public:
    PlayStates();
    ~PlayStates() { };

    static Entity*  createTile(Map* map, const glm::vec3& pos, eArchetype type);
    static Entity*  createParticlesEmittor(const glm::vec3& pos, eArchetype type);
};
