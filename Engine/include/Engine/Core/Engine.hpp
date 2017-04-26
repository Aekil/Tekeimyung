/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>

#include <Engine/Core/GameStateManager.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Debug/DebugWindow.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Window/GameWindow.hpp>

class Engine
{
public:
    Engine();
    ~Engine();

    bool                                    init();
    bool                                    run(int ac, char** av, std::shared_ptr<GameState> startGameState);
    bool                                    stop();
    GameStateManager&                       getGameStateManager();

    template<typename T, typename... Args>
    void                                    addDebugWindow(Args... args)
    {
        std::shared_ptr<T> debugWindow = std::make_shared<T>(args...);

        if (getDebugWindow<T>() != nullptr)
        {
            LOG_WARN("Engine::addDebugWindow: Attempt to add debug window \"%s\" that is already added", debugWindow->getTitle().c_str());
            return;
        }

        _debugWindows.push_back(debugWindow);
    }

    template<typename T>
    void                                    addDebugWindow(std::shared_ptr<T> newDebugWindow)
    {
        _debugWindows.push_back(newDebugWindow);
    }

    template<typename T>
    std::shared_ptr<T>                      getDebugWindow()
    {
        uint32_t id = T::identifier;

        for (auto& debugWindow: _debugWindows)
        {
            if (debugWindow->getId() == id)
            {
                return (std::static_pointer_cast<T>(debugWindow));
            }
        }

        return (nullptr);
    }

    const std::vector<std::shared_ptr<DebugWindow>>& getDebugWindows() const;

    void                                    toggleDebugWindowsDisplay(bool displayed);

private:
    bool                                    initDebugWindows(int ac, char** av);
    bool                                    initStartGameState(std::shared_ptr<GameState> startGameState);

private:
    GameStateManager                        _gameStateManager;
    std::shared_ptr<GameWindow>             _window;
    std::shared_ptr<SoundManager>           _soundManager;
    std::shared_ptr<Renderer>               _renderer;
    std::shared_ptr<Logger>                 _logger;

    std::vector<std::shared_ptr<DebugWindow>> _debugWindows;
};
