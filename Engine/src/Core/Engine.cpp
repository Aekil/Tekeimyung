#include "Core/Engine.hpp"
#include "Utils/Timer.hpp"

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
    // FPS counter
    Timer               timer;
    double              timePerFrame;

    timePerFrame = 1.0f / 60.0f;
    while (1)
    {
        // Run one frame each 16ms
        if (timer.getElapsedTime() >= timePerFrame)
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
            timer.reset();
        }
    }
    return (true);
}

bool    Engine::stop()
{
    return (true);
}