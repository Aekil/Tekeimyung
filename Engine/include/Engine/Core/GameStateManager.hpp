#pragma once

#include <memory>
#include <vector>

#include <Engine/Core/GameState.hpp>

class GameState;

class GameStateManager
{
public:
    GameStateManager();
    virtual ~GameStateManager();

    template<typename T>
    bool                                        addState()
    {
        std::shared_ptr<T> gameState = std::make_shared<T>(this);
        gameState->onEnter();
        if (!gameState->init() || !gameState->initSystems())
        {
            return (false);
        }

        _states.push_back(gameState);

        return (true);
    }

    void                                        removeCurrentState();
    std::shared_ptr<GameState>                  getCurrentState() const;
    bool                                        hasStates() const;

    std::vector<std::shared_ptr<GameState>>&    getStates()
    {
        return (_states);
    }

private:
    // States vector
    std::vector<std::shared_ptr<GameState>>     _states;
};
