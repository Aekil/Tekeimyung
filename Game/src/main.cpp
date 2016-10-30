#include "GameStates/PlayState.hpp"

#include "Core/Engine.hpp"
#include "EntityFactory.hpp"
#include "Utils/Exception.hpp"

int     main()
{
    Engine engine;
    auto &&gameStateManager = engine.getGameStateManager();

    try
    {
        if (!engine.init())
            return (1);

        // Load entities after engine initialization to have logs
        EntityFactory::loadDirectory("ressources/archetypes");

        if (!gameStateManager.addState<PlayState>())
            return (1);
        else if (!engine.run())
            return (1);
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    engine.stop();
    return (0);
}
