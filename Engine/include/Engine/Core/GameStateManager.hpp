/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <vector>

#include <ECS/EntityManager.hpp>
#include <Engine/Core/GameState.hpp>
#include <Engine/Debug/Debug.hpp>
#include <Engine/Debug/Logger.hpp>

class GameState;

class GameStateManager
{
public:
    GameStateManager();
    virtual ~GameStateManager();

    template<typename T>
    bool                                        addState(T gameState, EntityManager* em = nullptr)
    {
        gameState->bindEntityManager();
        gameState->setupSystems();
        gameState->loadLevel();

        // Copy all entities from EntityManager into the gameState
        if (em)
        {
            gameState->cloneEntityManager(em);
        }

        if (!gameState->init() || !gameState->initSystems())
        {
            return (false);
        }

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
    bool                                        addState(EntityManager* em = nullptr)
    {
        std::shared_ptr<T> gameState = std::make_shared<T>(this);
        return (addState(gameState, em));
    }

    template<typename T>
    bool                                        replaceState(T gameState)
    {
        if (!removeCurrentState())
        {
            LOG_WARN("Can't replace state: there is no states in the stack");
            return (false);
        }
        return (addState(gameState));
    }

    template<typename T>
    bool                                        replaceState()
    {
        std::shared_ptr<T> gameState = std::make_shared<T>(this);
        return (replaceState(gameState));
    }

    bool                                        removeCurrentState();

    std::shared_ptr<GameState>                  getCurrentState() const;
    bool                                        hasStates() const;

    std::vector<std::shared_ptr<GameState>>&    getStates()
    {
        return (_states);
    }

    void                                        clearStates();
    void                                        removeLastStates(uint32_t nb);

private:
    // States vector
    std::vector<std::shared_ptr<GameState>>     _states;
};
