#pragma once

#include <memory>
#include <list>

#include <Engine/Core/GameState.hpp>

class GameStateManager
{
public:
    GameStateManager();
    virtual ~GameStateManager();

    template<typename T>
    bool                                        addState()
    {
        std::shared_ptr<T> gameState = std::make_shared<T>();
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

private:
    // States list
    std::list<std::shared_ptr<GameState>>       _states;
};
