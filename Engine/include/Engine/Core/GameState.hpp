#pragma once

#include <memory>
#include <vector>

#include <ECS/World.hpp>
#include <ECS/EntityManager.hpp>

#include <Engine/Core/GameStateManager.hpp>
#include <Engine/Utils/DebugWindow.hpp>

class GameStateManager;

class GameState
{
public:
    GameState(GameStateManager* gameStateManager, uint32_t id);
    virtual ~GameState();

    virtual bool            init() = 0;
    virtual bool            initSystems();
    virtual bool            update(float elapsedTime);

    uint32_t                getId() const;

    template<typename T, typename... Args>
    void                            addDebugWindow(Args... args)
    {
        std::shared_ptr<T> debugWindow = std::make_shared<T>(args...);
        _debugWindows.push_back(debugWindow);
    }

    template<typename T>
    void                            addDebugWindow(std::shared_ptr<T> newDebugWindow)
    {
        _debugWindows.push_back(newDebugWindow);
    }

protected:
    World                   _world;
    std::vector<std::shared_ptr<DebugWindow>> _debugWindows;

    GameStateManager*       _gameStateManager;

    uint32_t                _id;
};


template<typename GameStateType>
class BaseGameState: public GameState
{
    public:\
    BaseGameState(GameStateManager* gameStateManager): GameState(gameStateManager, GameStateType::identifier) {}
};

#define START_GAMESTATE(name) \
    class name : public BaseGameState<name> { \
        public:\
        name(GameStateManager* gameStateManager): BaseGameState(gameStateManager) {}\
        static constexpr unsigned int identifier = #name##_crc32;

#define END_GAMESTATE(name) \
    };
