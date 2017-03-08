/**
* @Author   Guillaume Labey
*/

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>

#include <Engine/BasicState.hpp>
#include <Engine/Core/Engine.hpp>
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

int     main(int ac, char** av)
{
    Engine engine;
    auto &&gameStateManager = engine.getGameStateManager();
    try
    {
        if (!engine.init("resources_engine"))
            return (1);

        // Load textures, models & sounds
        ResourceManager::getInstance()->loadResources("resources");
        ResourceManager::getInstance()->setSaveDirectory("resources");

        // Load entities after engine initialization to have logs
        EntityFactory::loadDirectory("resources/archetypes");

        // Load levels
        LevelLoader::getInstance()->loadDirectory("resources/levels");
        REGISTER_GAMESTATE(ConfirmExitState);
        REGISTER_GAMESTATE(HowToPlayState);
        REGISTER_GAMESTATE(OptionsMenuState);
        REGISTER_GAMESTATE(PauseState);
        REGISTER_GAMESTATE(PlayState);

        EventSound::loadEvents();
        GameWindow::getInstance()->registerCloseHandler(windowCloseHandler, &engine);

        std::shared_ptr<PlayState> playState = std::make_shared<PlayState>(&gameStateManager);

        if (!engine.run(ac, av, playState))
            return (1);
    }
    catch(const Exception& e)
    {
        LOG_ERROR("ERROR: %s", e.what());
    }

    engine.stop();
    return (0);
}
