/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <vector>

#include <Engine/Core/GameState.hpp>
#include <Engine/Utils/Debug.hpp>

class GameState;

class GameStateManager
{
public:
    GameStateManager();
    virtual ~GameStateManager();

    template<typename T>
    bool                                        addState(T gameState)
    {
        gameState->bindEntityManager();
        gameState->setupSystems();
        gameState->loadLevel();
        if (!gameState->init() || !gameState->initSystems())
        {
            return (false);
        }

        // Don't add debug windows if we are not in debug mode
        #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
            gameState->initDebugWindows();
        #endif

        gameState->onEnter();

        // Bind the entity manager of current state
        // in case the new GameState was add in GameState::update and update is not finished
        // (The EntityManager of the new GameState could conflict with the current GameState)
        if (hasStates())
        {
            getCurrentState()->bindEntityManager();
        }
        _states.push_back(gameState);

        return (true);
    }

    template<typename T>
    bool                                        addState()
    {
        std::shared_ptr<T> gameState = std::make_shared<T>(this);
        return (addState(gameState));
    }

    void                                        removeCurrentState();
    void                                        removeBackState();

    std::shared_ptr<GameState>                  getCurrentState() const;
    bool                                        hasStates() const;

    std::vector<std::shared_ptr<GameState>>&    getStates()
    {
        return (_states);
    }

    void                                        clearStates();

private:
    // States vector
    std::vector<std::shared_ptr<GameState>>     _states;
};
