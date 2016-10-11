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
        EntityFactory::init("ressources/archetypes");

        if (!engine.init() || !gameStateManager.addState<PlayState>())
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
