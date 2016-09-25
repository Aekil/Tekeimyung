#pragma once

#include <memory>
#include "GameStateManager.hpp"

class Engine
{
public:
    Engine();
    ~Engine();

    bool                    init();
    bool                    run();
    bool                    stop();

private:
    GameStateManager        _gameStateManager;
};