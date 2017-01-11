/**
* @Author   Julien Chardon
*/

#pragma once

#include <Engine/EntityFactory.hpp>
#include <Game/Map.hpp>

class PlayStates
{
public:
    PlayStates();
    ~PlayStates() { };

    static Entity*  createTile(eArchetype type, const glm::vec3& pos);
    static Entity*  createTile(eArchetype type);
};
