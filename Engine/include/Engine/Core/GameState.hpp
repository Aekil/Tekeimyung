#pragma once

#include <memory>
#include <vector>

#include <ECS/World.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Utils/DebugWindow.hpp>

class GameState
{
public:
    GameState();
    virtual ~GameState();

    virtual bool            init() = 0;
    virtual bool            initSystems();
    virtual bool            update(float elapsedTime);

    template<typename T, typename... Args>
    void                            addDebugWindow(Args... args)
    {
        std::shared_ptr<T> debugWindow = std::make_shared<T>(args...);
        _debugWindows.push_back(debugWindow);
    }

protected:
    World                   _world;
    std::vector<std::shared_ptr<DebugWindow>> _debugWindows;
};
