#include "Core/Engine.hpp"

Engine::Engine() {}

Engine::~Engine() {}

bool    Engine::init()
{
    // TODO: Create window

    // TODO: Add State

    return (true);
}

bool    Engine::run()
{
    while (1)
    {
        if (!_gameStateManager.hasStates())
        {
            return (true);
        }

        auto &&currentState = _gameStateManager.getCurrentState();

        if (!currentState->update(0))
        {
            _gameStateManager.removeCurrentState();
        }
    }
    return (true);
}

bool    Engine::stop()
{
    return (true);
}