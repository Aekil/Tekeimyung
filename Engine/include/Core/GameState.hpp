#pragma once

#include "World.hpp"
#include "EntityManager.hpp"

class GameState
{
 public:
    GameState();
    virtual ~GameState();

    virtual bool                    init() = 0;
    virtual bool                    initSystems();
    virtual bool                    update(float elapsedTime);

protected:
    World                           _world;
};