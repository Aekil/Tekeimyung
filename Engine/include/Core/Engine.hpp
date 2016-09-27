#pragma once

#include <memory>
#include "GameStateManager.hpp"
#include "Window/GameWindow.hpp"

class Engine
{
public:
    Engine();
    ~Engine();

    bool                                    init();
    bool                                    run();
    bool                                    stop();

private:
    GameStateManager                        _gameStateManager;
    std::shared_ptr<GameWindow>             _window;
};