#include <iostream>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Core/Engine.hpp>

#include <Game/EntityFactory.hpp>
#include <Game/GameStates/PlayState.hpp>

int     main()
{
    Engine engine;
    auto &&gameStateManager = engine.getGameStateManager();

    try
    {
        if (!engine.init())
            return (1);

        // Load entities after engine initialization to have logs
        EntityFactory::loadDirectory(ARCHETYPES_LOCATION);
        // Load textures and models
        RessourceManager::getInstance()->loadResources("resources");

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
