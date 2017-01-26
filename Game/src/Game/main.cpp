/**
* @Author   Guillaume Labey
*/

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>

#include <Engine/Core/Engine.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/EventSound.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Game/GameStates/PlayState.hpp>
#include <Game/GameStates/ConfirmExitState.hpp>

void    windowCloseHandler(void* data)
{
    Engine* engine = static_cast<Engine*>(data);

    engine->getGameStateManager().addState<ConfirmExitState>();
}

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
        // Load textures, models & sounds
        RessourceManager::getInstance()->loadResources("resources");
        // Load levels
        LevelLoader::getInstance()->loadDirectory(LEVELS_DIRECTORY);
        EventSound::loadEvents();
        GameWindow::getInstance()->registerCloseHandler(windowCloseHandler, &engine);

        if (!gameStateManager.addState<EditorState>())
            return (1);
        else if (!engine.run())
            return (1);
    }
    catch(const Exception& e)
    {
        LOG_ERROR("ERROR: %s", e.what());
    }

    engine.stop();
    return (0);
}
