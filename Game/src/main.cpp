#include "GameStates/PlayState.hpp"

#include "Core/Engine.hpp"

int     main()
{
    Engine engine;
    auto &&gameStateManager = engine.getGameStateManager();

    if (!engine.init() || !gameStateManager.addState<PlayState>())
        return (1);
    else if (!engine.run())
        return (1);

    engine.stop();
    return (0);
}