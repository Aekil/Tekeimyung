#pragma once

#include <memory>
#include <list>
#include "GameState.hpp"

class GameStateManager
{
<<<<<<< HEAD
public:
    GameStateManager();
    virtual ~GameStateManager();
=======
 public:
    GameStateManager();
    ~GameStateManager();
>>>>>>> 283ac34c1adff2944fe0e528fd81d98014c88b39

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
<<<<<<< HEAD
};
=======
};
>>>>>>> 283ac34c1adff2944fe0e528fd81d98014c88b39
