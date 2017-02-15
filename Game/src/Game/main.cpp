/**
* @Author   Guillaume Labey
*/

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>

#include <Engine/BasicState.hpp>
#include <Engine/Core/Engine.hpp>
#include <Engine/EditorState.hpp>
#include <Engine/EditorMenuDebugWindow.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/EventSound.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>
#include <Game/GameStates/PauseState.hpp>
#include <Game/GameStates/PlayState.hpp>

void    windowCloseHandler(void* data)
{
    Engine* engine = static_cast<Engine*>(data);
    auto& gameStateManager = engine->getGameStateManager();

    // Only add ConfirmExitState if not already in the states
    if (gameStateManager.getCurrentState() &&
        gameStateManager.getCurrentState()->getId() != ConfirmExitState::identifier)
    {
        gameStateManager.addState<ConfirmExitState>();
    }
}

bool initGameState(int ac, char** av, GameStateManager* gameStateManager)
{
    std::shared_ptr<GameState> state = nullptr;

    #if defined(ENGINE_DEBUG) && ENGINE_DEBUG == true
        state = std::make_shared<EditorState>(gameStateManager);
    #else
        state = std::make_shared<PlayState>(gameStateManager);
    #endif

    if (!gameStateManager->addState(state))
    {
        return (false);
    }

    if (ac >= 2)
    {
        // AV[1] => Level name (Ex: "Options")
        if (!LevelLoader::getInstance()->hasLevel(av[1]))
        {
            LOG_ERROR("Can't find level \"%s\"", av[1]);
            return (false);
        }

        auto debugMenu = state->getDebugWindow<EditorMenuDebugWindow>();
        ASSERT(debugMenu != nullptr, "EditorState should have EditorMenuDebugWindow");
        debugMenu->loadLevel(av[1]);

        // AV[2] => options (Ex: "-a")
        if (ac >= 3)
        {
            // Auto play level
            if (av[2] == std::string("-a"))
            {
                debugMenu->play();
            }
        }
    }

    return (true);
}

int     main(int ac, char** av)
{
    Engine engine;
    auto &&gameStateManager = engine.getGameStateManager();
    try
    {
        if (!engine.init())
            return (1);

        // Load textures, models & sounds
        ResourceManager::getInstance()->loadResources("resources");

        // Load geometries: plane, sphere, box, circle
        GeometryFactory::initGeometries();

        // Load entities after engine initialization to have logs
        EntityFactory::loadDirectory(ARCHETYPES_LOCATION);


        // Load levels
        LevelLoader::getInstance()->loadDirectory(LEVELS_DIRECTORY);
        REGISTER_GAMESTATE(ConfirmExitState);
        REGISTER_GAMESTATE(HowToPlayState);
        REGISTER_GAMESTATE(OptionsMenuState);
        REGISTER_GAMESTATE(PauseState);
        REGISTER_GAMESTATE(PlayState);

        EventSound::loadEvents();
        GameWindow::getInstance()->registerCloseHandler(windowCloseHandler, &engine);

        if (!initGameState(ac, av, &gameStateManager))
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
