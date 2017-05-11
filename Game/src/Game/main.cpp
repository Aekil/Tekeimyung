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
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Debug/Debug.hpp>

#include <Game/GameStates/ConfirmBackToMenuState.hpp>
#include <Game/GameStates/ConfirmExitState.hpp>
#include <Game/GameStates/HowToPlayState.hpp>
#include <Game/GameStates/BuildingListState.hpp>
#include <Game/GameStates/OptionsMenuState.hpp>
#include <Game/GameStates/PauseState.hpp>
#include <Game/GameStates/PlayState.hpp>
#include <Game/GameStates/HomeScreenState.hpp>
#include <Game/GameStates/VictoryScreenState.hpp>
#include <Game/GameStates/DefeatScreenState.hpp>

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
        REGISTER_GAMESTATE(ConfirmBackToMenuState);
        REGISTER_GAMESTATE(ConfirmExitState);
        REGISTER_GAMESTATE(HowToPlayState);
        REGISTER_GAMESTATE(BuildingListState);
        REGISTER_GAMESTATE(OptionsMenuState);
        REGISTER_GAMESTATE(PauseState);
        REGISTER_GAMESTATE(PlayState);
        REGISTER_GAMESTATE(HomeScreenState);
        REGISTER_GAMESTATE(VictoryScreenState);
        REGISTER_GAMESTATE(DefeatScreenState);

        EventSound::loadEvents();
        GameWindow::getInstance()->registerCloseHandler(windowCloseHandler, &engine);

        //  Before, we were playing the PlayState at first, now we have to play the HomeScreenState !
        //  std::shared_ptr<PlayState> playState = std::make_shared<PlayState>(&gameStateManager);
        //
        //  if (!engine.run(ac, av, playState))
        //      return (1);

        std::shared_ptr<HomeScreenState>    homeScreenState = std::make_shared<HomeScreenState>(&gameStateManager);

        if (!engine.run(ac, av, homeScreenState))
            return (1);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Exception : %s", e.what());
    }

    engine.stop();
    return (0);
}
