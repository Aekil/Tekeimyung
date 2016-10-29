#pragma once

#include <memory>
#include "GameStateManager.hpp"
#include "Window/GameWindow.hpp"
#include "Utils/Logger.hpp"

class Engine
{
public:
    Engine();
    ~Engine();

    bool                                    init();
    bool                                    run();
    bool                                    stop();
    GameStateManager&                       getGameStateManager();

private:
    GameStateManager                        _gameStateManager;
    std::shared_ptr<GameWindow>             _window;
    std::shared_ptr<Logger>                 _logger;
};
